//
//  CSocketConnectWork.cpp
//  NetworkEngine
//
//  Created by 周贺伟 on 2017/3/13.
//  Copyright © 2017年 周贺伟. All rights reserved.
//

#include "CSocketConnectWork.hpp"
#include "SocketEngineManager.h"
#include "DataRequestProtocol.h"
#include "MessageAdapter.h"

SocketConnectWork::SocketConnectWork(_DataHubMessageKind kind, _HANDLE socketHandler, _string& ip, _UINT port, _CONST _INT timeout_sec)
:m_socketHandler(socketHandler)
,m_ip(ip)
,m_port(port)
,m_kind( kind )
,m_bIsRunning(_TRUE)
,m_timeout_sec(timeout_sec)
{
}

SocketConnectWork::~SocketConnectWork()
{
}

_VOID SocketConnectWork::doWork()
{
#ifdef DEBUG
    printf("%s connect!\n", __FUNCTION__);
#endif
    _INT code = SocketEngineManager::connectToHost(m_socketHandler, m_ip.c_str(), m_port, m_timeout_sec);
    
    if (!m_bIsRunning) {
        return;
    }
    
    _HANDLE bundleHandler = createEventInfoBundle(m_kind, code, (_PVOID)m_socketHandler, _NULL, 0);
    postMessage( _SERVICE_DATAHUB, MESSAGE_DATAHUB_ON_EVENT, &bundleHandler, sizeof(bundleHandler));
}

_VOID SocketConnectWork::cancelWork()
{
    m_bIsRunning = _FALSE;
    SocketEngineManager::disconnectSocket(m_socketHandler);
}
