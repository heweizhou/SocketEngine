//
//  CSocketWork.cpp
//  NetworkEngine
//
//  Created by 周贺伟 on 2017/3/13.
//  Copyright © 2017年 fxiaoke. All rights reserved.
//

#include "CSocketWork.hpp"
#include "MessageAdapter.h"
#include "SocketEngineManager.h"
#include "DataRequestProtocol.h"

//SocketWork::SocketWork( _DataHubMessageKind kind, _HANDLE socketHandler, _HANDLE bufferHandler, _PVOID param, _ULONG param_len)
SocketWork::SocketWork(_DataHubMessageKind kind, _HANDLE socketHandler, IFSCallBack* cb, _PVOID param, _ULONG param_len)
:m_socketHandler(socketHandler)
,m_cb(cb)
,m_kind(kind)
,m_bIsRunning(_TRUE)
{
    if (param_len != 0) {
        m_param = malloc(sizeof(param_len));
        if (m_param != _NULL) {
            SAFE_COPY(m_param, param_len, param, param_len);
            m_param_len = param_len;
        }
    } else {
        m_param = _NULL;
        m_param_len = 0;
    }
}

SocketWork::~SocketWork()
{
    SAFE_FREE(m_param);
//    ShareMemManager->destroySharedMemory(m_bufferHandler);
}

_VOID SocketWork::doWork()
{
//    ICSharedMemory* shm = ShareMemManager->getObject(m_bufferHandler);
//    _PVOID buffer = shm->obtainMemoryLock();
//    _ULONG bufferSize = shm->getlength();
    
    _CHAR* buffer = _NULL;
    _ULONG size = 0;
    
    if (!m_cb) {
        return ;
    }
    
    if (!m_bIsRunning) {
        return;
    }
    
    _UINT code = INNER_EVENT_DEFAULT;
    
    if(m_cb->socketSendRequestData((_INT)m_socketHandler, m_param, &buffer, size)){
        
        _LONG ret = SocketEngineManager::sendData(m_socketHandler, (_CONST _CHAR*)buffer, size);
        if (ret > 0) {
            code = INNER_EVENT_SEND_SUCCESSED;
        } else {
            code = INNER_EVENT_SEND_FAILD;
        }
        
    } else {
        code = INNER_EVENT_SEND_PACKAGE_INVALID;
    }
    
    if (!m_bIsRunning) {
        return;
    }
    
    _HANDLE bundleHandler = createEventInfoBundle(m_kind, code, (_PVOID)m_socketHandler, m_param, m_param_len);
    postMessage( _SERVICE_DATAHUB, MESSAGE_DATAHUB_ON_EVENT, &bundleHandler, sizeof(bundleHandler));
    
//    shm->releaseMemoryLock();
//    ShareMemManager->destroySharedMemory(m_bufferHandler);
}

_VOID SocketWork::cancelWork()
{
    m_bIsRunning = _FALSE;
    SocketEngineManager::disconnectSocket(m_socketHandler);
    
}
