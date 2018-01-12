// CThreadPool.cpp : ∂®“Âøÿ÷∆Ã®”¶”√≥Ã–Úµƒ»Îø⁄µ„°£
//

#include "CThreadPool.h"
#include "CThreadFactory.h"
#include "CMutexFactory.h"
#include "CSemaphoreManager.h"
#include "ThreadWithQueue.hpp"
#include <algorithm>
#include <sstream>


#define DEFAULT_WORKQUEUE_SIZE		30000
#define DEFAULT_WORKTHREAD_COUNT	2

namespace  _MY_NAME_SPACE_
{
    
    _PVOID CThreadPool::m_thread_obj = _NULL;
    
    _VOID CThreadPool::AddWork(IWork* task, _BOOL isImmediately)
    {
        if (m_thread_obj != _NULL) {
            ((CThreadWithQueue*)m_thread_obj)->AddWork(task, isImmediately);
        }
    }
    
    _VOID CThreadPool::start(_INT threadCount)
    {
        if (m_thread_obj == _NULL) {
            CThreadWithQueue* thread_queue = new CThreadWithQueue(DEFAULT_WORKQUEUE_SIZE);
            thread_queue->setDelayPerTask(0);
            thread_queue->setMaxWorkThread(threadCount);
            m_thread_obj = thread_queue;
        }else{
            ((CThreadWithQueue*)m_thread_obj)->clearWorkQueue();
        }
        
        
        ((CThreadWithQueue*)m_thread_obj)->start();
    }
    
    _VOID CThreadPool::start()
    {
        CThreadPool::start(DEFAULT_WORKTHREAD_COUNT);
    }

    _VOID CThreadPool::stop()
    {
        if (m_thread_obj != _NULL) {
            ((CThreadWithQueue*)m_thread_obj)->stop();
        }
    }
    
    _BOOL CThreadPool::getStatus()
    {
        if (m_thread_obj != _NULL) {
            return ((CThreadWithQueue*)m_thread_obj)->getStatus();
        }
        
        return _FALSE;
    }
    
    _VOID CThreadPool::clearWorkQueue()
    {
        if (m_thread_obj != _NULL) {
            ((CThreadWithQueue*)m_thread_obj)->clearWorkQueue();
        }
    }
}
