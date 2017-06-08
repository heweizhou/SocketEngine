//
//  SocketRecvManager.cpp
//  SocketEngine
//
//  Created by 周贺伟 on 16/8/18.
//  Copyright © 2016年 周贺伟. All rights reserved.
//

#include "SocketRecvManager.h"
#include "CThreadFactory.h"
#include "DataRequestProtocol.h"
#include "MessageAdapter.h"
#include "CSmartPointer.h"
#include <unistd.h>

#define RECV_BUFFER  1024
#define	max(a,b) ((a) > (b) ? (a) : (b))


SocketRecvManager*    SocketRecvManager::m_sInstance = _NULL;
ICMutex*              SocketRecvManager::m_sInstanceMutex = CMutexFactory::createStaticMutex();

SocketRecvManager* SocketRecvManager::getInstance()
{
    if (m_sInstance == _NULL) {
        scope_lock<ICMutex> lk(m_sInstanceMutex);
        if ( m_sInstance == _NULL )
        {
            m_sInstance = new SocketRecvManager();
        }
    }
    
    return m_sInstance;
}

_VOID SocketRecvManager::destroy()
{
    SAFE_DELETE(m_sInstance);
}

SocketRecvManager::SocketRecvManager()
{
    m_bRun = _FALSE;
    m_socketList.reserve(100);
    m_socketListMutex = CMutexFactory::createMutex();
    m_mutex = CMutexFactory::createMutex();
}

SocketRecvManager::~SocketRecvManager()
{
    stop();
    CThreadFactory::waitForThread(m_pid);
    SAFE_DELETE(m_socketListMutex);
    SAFE_DELETE(m_mutex);
}

_VOID SocketRecvManager::start(_DataHubMessageKind kind )
{
    m_kind = kind;
    scope_lock<ICMutex> lk(m_mutex);
    if (m_bRun == _FALSE) {
        m_bRun = _TRUE;
        CThreadFactory::createThreadWithName("doRecv", &m_pid, SocketRecvManager::doRecv, this, HIGH);
    }
}


_VOID SocketRecvManager::stop()
{
    scope_lock<ICMutex> lk(m_mutex);
    if (m_bRun == _TRUE) {
        SocketList_Clean();
        m_bRun = _FALSE;
    }
}

_BOOL SocketRecvManager::isRuning()
{
    scope_lock<ICMutex> lk(m_mutex);
    return m_bRun;
}

_VOID SocketRecvManager::dataBack(_INT fd, _PVOID buffer, _ULONG bufferSize)
{
    _HANDLE bundleHandler = createDataBackBundleByBuffer(m_kind, buffer, bufferSize, (_PVOID)(_LONG)fd, NULL, 0);
    postMessage(_SERVICE_DATAHUB, MESSAGE_DATAHUB_LOAD_COMPLETE, &bundleHandler, sizeof(bundleHandler));
}

_VOID SocketRecvManager::onEvent(_INT fd, _UINT eventCode)
{
    _HANDLE bundleHandler = createEventInfoBundle(m_kind, eventCode, (_PVOID)(_LONG)fd, NULL, 0);
    postMessage(_SERVICE_DATAHUB, MESSAGE_DATAHUB_ON_EVENT, &bundleHandler, sizeof(bundleHandler));
}

_VOID SocketRecvManager::private_SocketList_Del(_INT fd)
{
    auto it = std::find(m_socketList.begin(), m_socketList.end(), fd);
    if (it != m_socketList.end()) {
        m_socketList.erase(it);
    }
}

_VOID SocketRecvManager::SocketList_Add(_INT fd)
{
    scope_lock<ICMutex> lk(m_socketListMutex);
    m_socketList.push_back(fd);
}

_VOID SocketRecvManager::SocketList_Del(_INT fd)
{
    scope_lock<ICMutex> lk(m_socketListMutex);
    this->private_SocketList_Del(fd);
}



_VOID SocketRecvManager::SocketList_Clean()
{
    scope_lock<ICMutex> lk(m_socketListMutex);
    for (std::vector<_INT>::iterator it = m_socketList.begin(); it != m_socketList.end(); ++it)
    {
        close(*it);
    }
    
    m_socketList.clear();
}


_INT SocketRecvManager::FillSet(fd_set* RSet, fd_set* WSet, fd_set* ESet)
{
    scope_lock<ICMutex> lk(m_socketListMutex);
    
    if (RSet) {
        FD_ZERO(RSet);
    }
    
    if (WSet) {
        FD_ZERO(WSet);
    }
    
    if (ESet) {
        FD_ZERO(ESet);
    }
    
    _INT max = -1;
    for (std::vector<_INT>::iterator it = m_socketList.begin(); it != m_socketList.end(); ++it)
    {
        //if valid socket descriptor then add to read list
        if(*it > 0)
        {
            if (RSet) {
                FD_SET(*it, RSet);
            }
            
            if (WSet) {
                FD_SET(*it, WSet);
            }
            
            if (ESet) {
                FD_SET(*it, ESet);
            }
            
            max = max(max, (int)*it);
        }
    }
    
    return max;
}

_VOID SocketRecvManager::ProcessSelectEvent(fd_set* RSet, fd_set* WSet, fd_set* ESet)
{
    
    std::vector<_UINT> avaliable;
    std::vector<_UINT> unavaliable;
    avaliable.reserve(20);
    unavaliable.reserve(20);
    
    m_socketListMutex->lock();
    for (std::vector<_INT>::iterator it = m_socketList.begin(); it != m_socketList.end(); ++it)
    {
        _UINT fd = *it;
                
        if (FD_ISSET(fd , RSet))
        {
            avaliable.push_back((*it));
        }
        
        if ( FD_ISSET(fd, ESet ) )
        {
            onEvent(fd, INNER_EVENT_SOCKET_CLOSED);
            unavaliable.push_back(fd);
        }
    }
    
    for (std::vector<_UINT>::iterator it = unavaliable.begin(); it != unavaliable.end(); ++it)
    {
        private_SocketList_Del(*it);
    }
    
    m_socketListMutex->unlock();
    
    for (std::vector<_UINT>::iterator it = avaliable.begin(); it != avaliable.end(); ++it)
    {
        _UINT fd = *it;
        char recBuffer[RECV_BUFFER];
        _LONG recvBytes = read((int)fd, recBuffer, RECV_BUFFER);
        
        if (recvBytes > 0) {
            dataBack(fd, recBuffer, recvBytes);
        }else{
            onEvent(fd, INNER_EVENT_SOCKET_CLOSED);
            SocketList_Del(fd);
        }
    }
    
   
}

_PVOID SocketRecvManager::doRecv(_PVOID arg)
{
    fd_set readFD;            //关注的可读文件描述符集合
    fd_set exceptionFD;     //关注的异常文件描述符集合
    
    struct timeval overtime;
    overtime.tv_sec = 0;
    overtime.tv_usec = 500 * 1000;
    
    SocketRecvManager* mgr = (SocketRecvManager*)arg;
    mgr->incRef();
    
    while(mgr->isRuning())
    {
        //clear the socket set
        
        
        _INT max = mgr->FillSet(&readFD, _NULL, &exceptionFD);
        
        //wait for an activity on one of the sockets , timeout is NULL , so wait indefinitely
        int activity = select(max+1 , &readFD , NULL , &exceptionFD , &overtime);
        
        switch (activity) {
            case -1:
                break;
            case 0:
                continue;
                break;
            default:
            {
                mgr->ProcessSelectEvent(&readFD, _NULL, &exceptionFD);
            }
                break;
        }
    }
    
    mgr->release();
    return _NULL;
}
