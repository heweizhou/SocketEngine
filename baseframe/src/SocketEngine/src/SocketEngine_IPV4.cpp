////
////  SocketEngine.cpp
////  SocketEngine
////
////  Created by yangyanfei on 15/11/17.
////  Copyright © 2015年 fxiaoke. All rights reserved.
////
//
//#include "SocketEngine_IPV4.h"
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
//#include <fcntl.h>
//#include <netinet/tcp.h>
//#include <unistd.h>
//#include <netdb.h>
//#include <errno.h>
//
//#include <sys/time.h>
//#include <sys/ioctl.h>
//
//#ifdef MY_DEBUG
//#include "log.h"
//#endif
//
//#define SOCKET_CONNECT_FAILED  0
//#define RECV_BUFFER 1024
//#define SOCKET_CONNECT_SOCKOPT_VALUE  1
////#ifdef DEBUG
////void printHex(const char* buffer, unsigned long len){
////
////    printf("socket engine bin data start:\n");
////    unsigned char *ptr = (unsigned char*)buffer;
////    
////    for ( ; (char*)ptr - buffer < len; ++ptr) {
////        printf("%02X ", *ptr);
////    }
////    printf("\nsocket engine bin data end:\n");
////}
////#endif
//
//
//SocketEngine_IPV4::SocketEngine_IPV4(_DataHubMessageKind kind, _INT domain, _INT protocal)
//{
//    //create a socket fd
//    m_msgKind = kind;
//    m_socketHandler = socket(domain, protocal, 0);
//    m_socketMsgKind = 0;
//    m_isReady = _TRUE;
//    
//    m_mutex = CMutexFactory::createMutex();
//}
//
//SocketEngine_IPV4::~SocketEngine_IPV4()
//{
//    disconnectSocket();
//    SAFE_DELETE(m_mutex);
//}
//
//_DataHubMessageKind  SocketEngine_IPV4::getKind()
//{
//    return m_msgKind;
//}
//
//_VOID  SocketEngine_IPV4::setSocketMsgKind(_CONST _LONG msgKind)
//{
//    m_socketMsgKind = msgKind;
//}
//_LONG SocketEngine_IPV4::getSocketMsgKind()
//{
//    return m_socketMsgKind;
//}
//
//_HANDLE SocketEngine_IPV4::getHandler()
//{
//    scope_lock<ICMutex> lk(m_mutex);
//    return m_socketHandler;
//}
//
//
//_BOOL SocketEngine_IPV4::isConnected()
//{
////    scope_lock<ICMutex> lk(m_mutex);
//    if (m_socketHandler == 0) {
//        return _FALSE;
//    }
//    
//    struct sockaddr remoteAddr;
//    socklen_t socketLen = sizeof(remoteAddr);
//    if (getpeername((int)m_socketHandler, &remoteAddr, &socketLen) < 0)
//    {
//        return _FALSE;
//    }
//    return _TRUE;
//}
//
//_VOID SocketEngine_IPV4::setSendTimeOut(_CONST _LONG second)
//{
////    scope_lock<ICMutex> lk(m_mutex);
//    if (m_socketHandler) {
//        struct timeval timeout = {60,0};
//        timeout.tv_sec = second;
//        setsockopt((_INT)m_socketHandler, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(struct timeval));
//    }
//    
//}
//_VOID SocketEngine_IPV4::setRecvTimeOut(_CONST _LONG second)
//{
////    scope_lock<ICMutex> lk(m_mutex);
//    if (m_socketHandler) {
//        struct timeval timeout = {60,0};
//        timeout.tv_sec = second;
//        setsockopt((_INT)m_socketHandler, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(struct timeval));
//    }
//}
//
//_LONG SocketEngine_IPV4::sendData(_CONST _CHAR *binaryData,_LONG dataLength)
//{
////    scope_lock<ICMutex> lk(m_mutex);
//    
//#ifdef MY_DEBUG
//    _log("start send data!");
//#endif
//    
////#ifdef DEBUG
////    printHex(binaryData, dataLength);
////#endif
//    _LONG ret = write((int)m_socketHandler, binaryData, dataLength);
//#ifdef MY_DEBUG
//    _log("send data compelete!");
//#endif
//    return ret;
//
//}
//
//_LONG SocketEngine_IPV4::receiveData(_CONST _CHAR *binaryData, _LONG &dataLength)
//{
////    scope_lock<ICMutex> lk(m_mutex);
//    _LONG ret = read((int)m_socketHandler,(void*)binaryData, dataLength);
//    return ret;
//}
//
//
//_BOOL SocketEngine_IPV4::disconnectSocket()
//{
//    if (m_isReady) {
//        m_isReady = _FALSE;
//        scope_lock<ICMutex> lk(m_mutex);
//        if (m_socketHandler > 0 && close((int)m_socketHandler) < 0) {
//            return _FALSE;
//        }
//        
//    }
//    return _TRUE;
//}
