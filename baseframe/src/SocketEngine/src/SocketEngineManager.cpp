//
//  SocketEngineManager.cpp
//  SocketEngine
//
//  Created by YangYanfei on 15/11/16.
//  Copyright © 2015年 周贺伟. All rights reserved.
//

#include "SocketEngine_IPV4.h"
#include "SocketEngine_IPV6.h"

#include "SocketEngineManager.h"
#include "CThreadFactory.h"
#include "CMutexFactory.h"
#include "DataRequestProtocol.h"
#include "MessageAdapter.h"
#include "CSmartPointer.h"
#include "SocketRecvManager.h"

#include <errno.h>
#include <sys/socket.h>

#include "SocketFunctions.hpp"

#define SOCKET_CREATE_ERROR_CODE  -1



/////////////////////////
int socketErrorCode(int errorCode)
{
    int retErrorCode =-1;
    switch (errorCode) {
        case ETIMEDOUT:
            retErrorCode = INNER_EVENT_CONNECTED_TIMEOUT;
            break;
        case ECONNREFUSED:
            retErrorCode = INNER_EVENT_CONNECTED_REFUSED;
            break;
        case EADDRNOTAVAIL:
            retErrorCode = INNER_EVENT_CONNECTED_VAVAIL;
            break;
            
        default:
            retErrorCode = INNER_EVENT_CONNECTED_FAILD;
            break;
    }
    
    printf("errorCode:%d\n", retErrorCode);
    
    return retErrorCode;
}

/////////////////////////


_VOID SocketEngineManager::start_recv_thread(_DataHubMessageKind kind)
{
    SocketRecvManager::getInstance()->start(kind);
}

_VOID SocketEngineManager::stop_recv_thread()
{
    SocketRecvManager::getInstance()->stop();
}

_HANDLE SocketEngineManager::createSocket(_INT protocol, _INT type)
{
    return create_socket(protocol, type);
}

_INT SocketEngineManager::connectToHost(_HANDLE socketFD, _CONST _CHAR* ipAddress, _CONST _UINT port, _CONST _INT timeout_sec)
{
    //由于select处理返回错误的情况下，线程会退出，所有对于在次创建socket的时候需要重新启动接收数据的线程
//    startRunLoop();
        
    _socket_connect_status ret = connect_to_host((_UINT)socketFD, ipAddress, port, timeout_sec);
    
    switch (ret) {
        case _CONNECT_STATUS_SUCEESS:
        {
            
            SocketRecvManager::getInstance()->SocketList_Add((_UINT)socketFD);
            return INNER_EVENT_CONNECTED_SUCCESSED;
        }
            break;
        case _CONNECT_STATUS_FAILED:
        {
            return socketErrorCode(errno);
        }
            break;
        case _CONNECT_STATUS_TIMEOUT:
        {
            //                deleteSocketInsByKey(socketFD);
            return INNER_EVENT_CONNECTED_TIMEOUT;
        }
            break;
        case _CONNECT_STATUS_BREAK:
        {
            return INNER_EVENT_CONNECTED_USER_BREAK;
        }
        default:
            assert(0);
            break;
    }
    
    return INNER_EVENT_CONNECTED_FAILD;
}

_BOOL SocketEngineManager::isConnected(_HANDLE socketFD)
{
    return is_connected((_UINT)socketFD);
}


_LONG SocketEngineManager::sendData(_HANDLE socketFD,_CONST _CHAR *binaryData,_LONG dataLength)
{
    return send_data((_UINT)socketFD, binaryData, dataLength);
}

_LONG SocketEngineManager::receiveData(_HANDLE socketFD, _CHAR *binaryData, _LONG dataLength)
{
    return receive_data((_UINT)socketFD, binaryData, dataLength);
}


_BOOL SocketEngineManager::disconnectSocket(_HANDLE socketFD)
{
    SocketRecvManager::getInstance()->SocketList_Del((_UINT)socketFD);
    return disconnect_socket((_UINT)socketFD);
}
