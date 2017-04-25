////
////  SocketEngine_IPV6.cpp
////  SocketEngine
////
////  Created by 周贺伟 on 16/6/2.
////  Copyright © 2016年 fxiaoke. All rights reserved.
////
//
//#include "SocketEngine_IPV6.h"
//
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
//
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
//SocketEngine_IPV6::SocketEngine_IPV6(_INT domain, _INT protocal)
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
//SocketEngine_IPV6::~SocketEngine_IPV6()
//{
////    SAFE_DELETE(m_param);
//    disconnectSocket();
//    SAFE_DELETE(m_mutex);
//}
//
//_DataHubMessageKind  SocketEngine_IPV6::getKind()
//{
//    return m_msgKind;
//}
//
//_VOID  SocketEngine_IPV6::setSocketMsgKind(_CONST _LONG msgKind)
//{
//    m_socketMsgKind = msgKind;
//}
//
//_LONG SocketEngine_IPV6::getSocketMsgKind()
//{
//    return m_socketMsgKind;
//}
//
//_HANDLE SocketEngine_IPV6::getHandler()
//{
//    scope_lock<ICMutex> lk(m_mutex);
//    return m_socketHandler;
//}
//
//_socket_connect_status SocketEngine_IPV6::connectToHost( _CONST _CHAR* ipAddress, _CONST _UINT port, _CONST _INT timeout_second)
//{
//    scope_lock<ICMutex> lk(m_mutex);
//    
//    if (!m_isReady) {
//        return _CONNECT_STATUS_BREAK;
//    }
//    
//    _socket_connect_status status = _CONNECT_STATUS_SUCEESS;
//    
//    struct sockaddr_in6 seraddr;
//    bzero(&seraddr, sizeof(seraddr));
//    seraddr.sin6_family = AF_INET6;
//    seraddr.sin6_port = htons(port);
//    inet_pton(AF_INET6, ipAddress, &seraddr.sin6_addr);
//    
//    
//    int value = SOCKET_CONNECT_SOCKOPT_VALUE;
//    setsockopt((int)m_socketHandler, SOL_SOCKET, SO_NOSIGPIPE, &value, sizeof(value));
//    
//    /*Setting socket to non-blocking mode */
//    _INT flags = fcntl((_INT)m_socketHandler, F_GETFL, 0);
//    fcntl((_INT)m_socketHandler, F_SETFL, flags | O_NONBLOCK);
//    
//    _INT res = connect((_INT)m_socketHandler, (struct sockaddr *)&seraddr, sizeof(struct sockaddr));
//    
//    if (res < 0){
//        
//        _INT            count = 0;
//        struct timeval  timeout = {1, 0};
//        fd_set          writeset;
//        
//        if (errno == EINPROGRESS)
//        {
//            while (1)
//            {
//                FD_ZERO(&writeset);
//                FD_SET((_INT)m_socketHandler, &writeset);
//                
//                _INT ret = select((_INT)m_socketHandler + 1, NULL, &writeset, NULL, &timeout);
//                if (ret == 0) {             //返回0，代表在描述词状态改变已超过timeout时间
//                    
//                    _INT error = -1;
//                    _INT len = sizeof(_INT);
//                    
//                    getsockopt((_INT)m_socketHandler, SOL_SOCKET, SO_ERROR, &error, (socklen_t *)&len);
//                    //                    if (error == 0) {         // 超时，可以做更进一步的处理，如重试等
//                    //                        status = _CONNECT_STATUS_TIMEOUT;
//                    //                    } else {
//                    //                        status = _CONNECT_STATUS_FAILED;
//                    //                        break;
//                    //                    }
//                    
//                    if (error != 0) {
//                        status = _CONNECT_STATUS_FAILED;
//                        break;
//                    }
//                    
//                } else if (ret == -1) {     // 返回-1， 有错误发生，错误原因存在于errno
//                    status = _CONNECT_STATUS_FAILED;
//                    break;
//                } else {                     // 成功，返回描述词状态已改变的个数
//                    status = _CONNECT_STATUS_SUCEESS;
//                    break;
//                }
//                
//                count++;
//                if (count >= timeout_second) {
//                    status = _CONNECT_STATUS_TIMEOUT;
//                    break;
//                }
//                
//                if (m_isReady == _FALSE) {
//                    status = _CONNECT_STATUS_BREAK;
//                    break;
//                }
//            }
//            
//        }else{
//            status = _CONNECT_STATUS_FAILED;
//        }
//        
//    } else {
//        status = _CONNECT_STATUS_SUCEESS;
//    }
//    
//    fcntl((_INT)m_socketHandler, F_SETFL, flags);
//    
//    return status;
//}
//
//_BOOL SocketEngine_IPV6::isConnected()
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
//_VOID SocketEngine_IPV6::setSendTimeOut(_CONST _LONG micsecond)
//{
////    scope_lock<ICMutex> lk(m_mutex);
//    if (m_socketHandler) {
//        _INT ms = (_INT)micsecond;
//        setsockopt((_INT)m_socketHandler, SOL_SOCKET, SO_SNDTIMEO, (char *)&ms, sizeof(int));
//    }
//    
//}
//_VOID SocketEngine_IPV6::setRecvTimeOut(_CONST _LONG micsecond)
//{
////    scope_lock<ICMutex> lk(m_mutex);
//    if (m_socketHandler) {
//        _INT ms = (_INT)micsecond;
//        setsockopt((_INT)m_socketHandler, SOL_SOCKET, SO_RCVTIMEO, (char *)&ms, sizeof(int));
//    }
//}
//
//_LONG SocketEngine_IPV6::sendData(_CONST _CHAR *binaryData,_LONG dataLength)
//{
////    scope_lock<ICMutex> lk(m_mutex);
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
//_LONG SocketEngine_IPV6::receiveData(_CONST _CHAR *binaryData, _LONG &dataLength)
//{
////    scope_lock<ICMutex> lk(m_mutex);
//    _LONG ret =  read((int)m_socketHandler,(void*)binaryData, dataLength);
//    return ret;
//}
//
//
//_BOOL SocketEngine_IPV6::disconnectSocket()
//{
//    if (m_isReady) {
//        m_isReady = _FALSE;
//        scope_lock<ICMutex> lk(m_mutex);
//        if (m_socketHandler > 0 && close((int)m_socketHandler) < 0) {
//            return _FALSE;
//        }
//    }
//    return _TRUE;
//}
