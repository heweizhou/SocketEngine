//
//  CThreadWithQueue.cpp
//  commons_mac
//
//  Created by 周贺伟 on 2016/11/23.
//  Copyright © 2016年 周贺伟. All rights reserved.
//

#include "ThreadWithQueue.hpp"
#include "CThreadFactory.h"
#include "CMutexFactory.h"
#include "CSemaphoreManager.h"
#include <algorithm>
#include <sstream>

#define DEFAULT_WORKQUEUE_SIZE		30000
#define DEFAULT_WORKTHREAD_COUNT	1

namespace  _MY_NAME_SPACE_
{
    class CThreadWithQueue::ConstInfo:public RefBase
    {
    public:
        ConstInfo(_UINT WorkQueueSize){
            
            assert(WorkQueueSize <= DEFAULT_WORKQUEUE_SIZE);
            
            m_ptrWorkQueueMutex     = CMutexFactory::createMutex();
            m_qWorkQueue            = CThreadWithQueue::WorkQueue();
            m_uiWorkQueueSize       = WorkQueueSize;
            m_delay_ms_per_task     = 0;
            
            _CHAR identify[255];
            sprintf(identify, "%s_%p", "thread_with_queue_1", this);
            m_ptrWorkAvaliable      = SemaphoreManager->createSemaphore(identify, 0);
            
            sprintf(identify, "%s_%p", "thread_with_queue_2", this);
            m_ptrQueueAvaliable     = SemaphoreManager->createSemaphore(identify, m_uiWorkQueueSize);
        }
        
        virtual ~ConstInfo(){
            _CHAR identify[255];
            sprintf(identify, "%s_%p", "thread_with_queue_1", this);
            SemaphoreManager->releaseSemaphore(identify);
            sprintf(identify, "%s_%p", "thread_with_queue_2", this);
            SemaphoreManager->releaseSemaphore(identify);
            
            SAFE_DELETE(m_ptrWorkQueueMutex);
        }
        
        _VOID AddWork(IWork* task, _BOOL isImmediately){
            
            if (!m_ptrQueueAvaliable) {
                return;
            }
            
            m_ptrQueueAvaliable->waitSem();
            
            if (m_ptrWorkAvaliable == _NULL) {
                return;
            }
            
            scope_lock<ICMutex> lk( m_ptrWorkQueueMutex );
            if (isImmediately == _TRUE) {
                m_qWorkQueue.push_front(task);
            }else{
                m_qWorkQueue.push_back(task);
            }
            m_ptrWorkAvaliable->postSem();
        }
        
        IWork* getWork(){
            IWork* ret = _NULL;
            
            m_ptrWorkQueueMutex->lock();
            if ( m_qWorkQueue.size() <= 0 ){
                m_ptrWorkQueueMutex->unlock();
                return ret;
            }
            ret = m_qWorkQueue.front();
            m_qWorkQueue.pop_front();
            m_ptrWorkQueueMutex->unlock();
            
            m_ptrQueueAvaliable->postSem();
            return  ret;
        }
        
        _VOID clearWorkQueue(){
            scope_lock<ICMutex> lk( m_ptrWorkQueueMutex );
            
            while (m_qWorkQueue.size() > 0){
                IWork* ret = m_qWorkQueue.front();
                m_qWorkQueue.pop_front();
                ret->release();
            }
        }
        
        _VOID setDelayPerTask(_UINT ms){
            m_delay_ms_per_task = ms;
        }
        
        _UINT getDelayPerTask(){
            return m_delay_ms_per_task;
        }
        
        _VOID wait(){
            m_ptrWorkAvaliable->waitSem();
        }
        
        _VOID stop(_UINT thread_count){
            m_ptrWorkAvaliable->postSem(thread_count);
        }
    private:
        WorkQueue                m_qWorkQueue;
        _UINT                    m_uiWorkQueueSize;
        ICMutex*                 m_ptrWorkQueueMutex;
        ICSemaphoreNamed*        m_ptrWorkAvaliable;
        ICSemaphoreNamed*        m_ptrQueueAvaliable;
        
        _UINT                    m_delay_ms_per_task;
    };
    
    
    class CThreadWithQueue::TheadInfo:public RefBase
    {
    public:
        TheadInfo(CThreadWithQueue::ConstInfo* cinfo)
        :m_pid(0)
        ,m_status(_TRUE)
        ,m_doingWork(_NULL)
        {
            cinfo->incRef();
            m_const_info = cinfo;
            m_mutex = CMutexFactory::createMutex();
        }
        virtual ~TheadInfo(){
            SAFE_RELEASE(m_const_info);
            SAFE_DELETE(m_mutex);
        }
    public:
        _PID&   getPID()
        {
            return m_pid;
        }
        
        _VOID   setDoingWork(IWork* work)
        {
            scope_lock<ICMutex> lk(m_mutex);
            m_doingWork = work;
        }
        
        _VOID   cancelDoingWork()
        {
            if (m_doingWork) {
                scope_lock<ICMutex> lk(m_mutex);
                m_doingWork->cancelWork();
                m_doingWork = _NULL;
            }
        }
        
        _VOID   going2Stop()
        {
            m_status = _FALSE;
        }
        
        _BOOL   getStatus()
        {
            return m_status;
        }
        
        CThreadWithQueue::ConstInfo* getConstInfo()
        {
            return m_const_info;
        }
        
    private:
        _PID    m_pid;
        _BOOL   m_status;
        IWork*  m_doingWork;
        
        ICMutex*    m_mutex;
        
        CThreadWithQueue::ConstInfo*  m_const_info;
    };
    
    
    CThreadWithQueue::CThreadWithQueue()
    {
        m_mThreadTaskMap = CThreadWithQueue::Thread_task_map();
        m_uiMaxThreadCount   = DEFAULT_WORKTHREAD_COUNT;
        m_bThreadPoolStatus  = _FALSE;
        
        m_const_info = new CThreadWithQueue::ConstInfo(DEFAULT_WORKQUEUE_SIZE);
    }
    
    CThreadWithQueue::CThreadWithQueue(_UINT QueueSize)
    {
        m_mThreadTaskMap = CThreadWithQueue::Thread_task_map();
        m_uiMaxThreadCount   = DEFAULT_WORKTHREAD_COUNT;
        m_bThreadPoolStatus  = _FALSE;
        m_const_info = new CThreadWithQueue::ConstInfo(QueueSize);
    }
    
    CThreadWithQueue::~CThreadWithQueue()
    {
        stop();
        SAFE_RELEASE(m_const_info);
    }
    
    _VOID CThreadWithQueue::AddWork(IWork* task, _BOOL isImmediately)
    {
        
        if( m_bThreadPoolStatus == _FALSE )
            return;
        
        m_const_info->AddWork(task, isImmediately);
        
    }
    
    _VOID CThreadWithQueue::clearWorkQueue()
    {
        m_const_info->clearWorkQueue();
    }
    
    _VOID CThreadWithQueue::setDelayPerTask(_UINT ms)
    {
        m_const_info->setDelayPerTask(ms);
    }
    
    _VOID CThreadWithQueue::setMaxWorkThread( _UINT maxThread )
    {
        m_uiMaxThreadCount = maxThread;
    }
    
    _VOID CThreadWithQueue::start()
    {
        if ( m_bThreadPoolStatus == _TRUE ){
            return;
        }
        
        m_bThreadPoolStatus = _TRUE;
        
        for ( _INT i = 0; i < m_uiMaxThreadCount; ++i )
        {
            CThreadWithQueue::TheadInfo* threadinfo = new CThreadWithQueue::TheadInfo(m_const_info);
            
            _TID pid = threadinfo->getPID();
            std::ostringstream os;
            os<<"WorkThread "<<i;
            CThreadFactory::createThreadWithName( os.str().c_str(), &pid, CThreadWithQueue::workThreadProc, threadinfo, LOW );
            m_mThreadTaskMap.insert(std::pair<_TID, TheadInfo*>(pid, threadinfo));
        }
    }
    
    _VOID CThreadWithQueue::stop()
    {
        if (m_bThreadPoolStatus == _FALSE) {
            return;
        }
        
        m_bThreadPoolStatus = _FALSE;
        
        clearWorkQueue();
        
        Thread_task_map::iterator it = m_mThreadTaskMap.begin();
        Thread_task_map::iterator end = m_mThreadTaskMap.end();
        for ( ; it != end; ++it) {
            if (it->second != _NULL) {
                it->second->going2Stop();
                it->second->cancelDoingWork();
                SAFE_RELEASE(it->second);
            }
        }
        
        m_const_info->stop((_INT)m_mThreadTaskMap.size());
        
        m_mThreadTaskMap.clear();
    }
    
    _BOOL CThreadWithQueue::getStatus()
    {
        return m_bThreadPoolStatus;
    }
        
    _PVOID CThreadWithQueue::workThreadProc(_PVOID param)
    {
        CThreadWithQueue::TheadInfo* threadinfo = static_cast<CThreadWithQueue::TheadInfo*>(param);
        threadinfo->incRef();
        
        CThreadWithQueue::ConstInfo* cinfo = threadinfo->getConstInfo();
        
        _UINT delay_ms_per_task = cinfo->getDelayPerTask();
        
        CThreadFactory::detachSelf();
        
        assert(threadinfo);
        do
        {
            cinfo->wait();
            
            if( threadinfo->getStatus() == _FALSE )
                break;
            
            if (delay_ms_per_task != 0) {
                CThreadFactory::sleep(delay_ms_per_task);
            }
            
            IWork* task = cinfo->getWork();
            
            if( task != _NULL)
            {
                threadinfo->setDoingWork(task);
                task->doWork();
            }
            
            threadinfo->setDoingWork(_NULL);
            SAFE_RELEASE(task);
            
            if( threadinfo->getStatus() == _FALSE )
                break;
            
        }while(1);
        threadinfo->release();
        return _NULL;
    }
}
