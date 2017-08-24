//
//  CSocketConnectWork.cpp
//  NetworkEngine
//
//  Created by 周贺伟 on 2017/3/13.
//  Copyright © 2017年 zhouhewei. All rights reserved.
//

#include "CSocketConnectWork.hpp"
#include "DataRequestProtocol.h"
#include "MessageAdapter.h"
#include "SocketFunctions.hpp"
#include "SocketUtility.hpp"
#include "SocketDefines.h"
#include "CBundleManager.h"
#include <errno.h>

SocketConnectWork::SocketConnectWork(_DataHubMessageKind kind, _HANDLE handler, _string& ip, _UINT port, _CONST _INT timeout_sec)
:m_outterHandler(handler)
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
    _INT code = INNER_EVENT_CONNECTED_FAILD;
    
    _HANDLE innerSocketHandler = -1;
    _socket_connect_status ret = connect_to_host(innerSocketHandler, m_ip.c_str(), m_port, m_timeout_sec);
    
    switch (ret) {
        case _CONNECT_STATUS_SUCEESS:
        {
            code = INNER_EVENT_CONNECTED_SUCCESSED;
        }
            break;
        case _CONNECT_STATUS_FAILED:
        {
            code = CSocketUtility::socketErrorCode(errno);
        }
            break;
        case _CONNECT_STATUS_TIMEOUT:
        {
            code = INNER_EVENT_CONNECTED_TIMEOUT;
        }
            break;
        case _CONNECT_STATUS_BREAK:
        {
            code = INNER_EVENT_CONNECTED_USER_BREAK;
        }
        default:
            assert(0);
            break;
    }
    
    
    if (!m_bIsRunning) {
        return;
    }
    
    _HANDLE bundleId = BundleManager->createBundle();
    if (bundleId) {
        CBundle* bundle = BundleManager->getObject(bundleId);
        bundle->setData("outterHandler", &m_outterHandler, sizeof(m_outterHandler));
        bundle->setData("innerHandler", &innerSocketHandler, sizeof(innerSocketHandler));
    }
    
    _HANDLE bundleHandler = createEventInfoBundle(m_kind, code, (_PVOID)bundleId, _NULL, 0);
    postMessage( _SERVICE_DATAHUB, MESSAGE_DATAHUB_ON_EVENT, &bundleHandler, sizeof(bundleHandler));
}

_VOID SocketConnectWork::cancelWork()
{
    m_bIsRunning = _FALSE;
}
