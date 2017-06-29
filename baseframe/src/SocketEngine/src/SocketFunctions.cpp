//
//  SocketFunctions.cpp
//  SocketEngine
//
//  Created by 周贺伟 on 2017/3/10.
//  Copyright © 2017年 周贺伟. All rights reserved.
//

#include "SocketFunctions.hpp"
#include "SocketUtility.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>

#include <sys/time.h>
#include <sys/ioctl.h>

#ifdef MY_DEBUG
#include "log.h"
#endif

#define SOCKET_CONNECT_FAILED  0
#define RECV_BUFFER 1024
#define SOCKET_CONNECT_SOCKOPT_VALUE  1

_UINT create_socket(_INT type)
{
    
    if (!CSocketUtility::isIpv4Net()) {
        return socket(AF_INET6, type, 0);
    }
    return socket(AF_INET, type, 0);
}

_socket_connect_status connect_to_host(_INT fd, _CONST _CHAR* ipAddress, _CONST _UINT port, _CONST _INT timeout_second)
{
    
    //兼容V4 v6
    _CHAR ip[128];
    socklen_t maxlen = 128;
    
    _PVOID  svraddr = nullptr;
    _INT    svraddr_len;
    
    struct sockaddr_in svraddr_4;
    struct sockaddr_in6 svraddr_6;

    //获取网络协议
    struct addrinfo *result;
    getaddrinfo(ipAddress, _NULL, _NULL, &result);
    _CONST struct sockaddr *sa = result->ai_addr;
    switch(sa->sa_family)
    {
        case AF_INET://ipv4
            
            bzero(&svraddr_4, sizeof(svraddr_4));
            svraddr_4.sin_family = AF_INET;
            
            if(inet_ntop(AF_INET, &(((struct sockaddr_in *)sa)->sin_addr), ip, maxlen) == _NULL){
                return _CONNECT_STATUS_FAILED;
            }
            
            svraddr_4.sin_addr.s_addr = inet_addr(ip);
            svraddr_4.sin_port = htons(port);
            memset(&(svraddr_4.sin_zero), 0, 8);
            svraddr_len = sizeof(svraddr_4);
            svraddr = &svraddr_4;
            break;
        case AF_INET6://ipv6
            
            if(inet_ntop(AF_INET6, &(((struct sockaddr_in6 *)sa)->sin6_addr), ip, maxlen) == _NULL){
                return _CONNECT_STATUS_FAILED;
            }
            
            bzero(&svraddr_6, sizeof(svraddr_6));
            svraddr_6.sin6_family = AF_INET6;
            
            if (inet_pton(AF_INET6, ip, &svraddr_6.sin6_addr) < 0 ) {
                perror(ip);
                return _CONNECT_STATUS_FAILED;
            }
            
            svraddr_6.sin6_port = htons(port);
            svraddr_len = sizeof(svraddr_6);
            svraddr = &svraddr_6;
            break;
            
        default:
            printf("Unknown AF\ns");
            return _CONNECT_STATUS_FAILED;
    }
    freeaddrinfo(result);
    
    ///connect 过程
    
    _socket_connect_status status = _CONNECT_STATUS_SUCEESS;
    
    int value = SOCKET_CONNECT_SOCKOPT_VALUE;
    setsockopt((int)fd, SOL_SOCKET, SO_NOSIGPIPE, &value, sizeof(value));
    /*Setting socket to non-blocking mode */
    _INT flags = fcntl((_INT)fd, F_GETFL, 0);
    fcntl((_INT)fd, F_SETFL, flags | O_NONBLOCK);
    
    _INT res = connect((_INT)fd, (struct sockaddr *)svraddr, svraddr_len);
    
    if (res < 0){
        
        _INT            count = 0;
        struct timeval  timeout = {1, 0};
        fd_set          writeset;
        
        if (errno == EINPROGRESS)
        {
            while (1)
            {
                FD_ZERO(&writeset);
                FD_SET((_INT)fd, &writeset);
                
                _INT ret = select((_INT)fd + 1, NULL, &writeset, NULL, &timeout);
                if (ret == 0) {             //返回0，代表在描述词状态改变已超过timeout时间
                    
                    _INT error = -1;
                    _INT len = sizeof(_INT);
                    
                    getsockopt((_INT)fd, SOL_SOCKET, SO_ERROR, &error, (socklen_t *)&len);
                    //                    if (error == 0) {         // 超时，可以做更进一步的处理，如重试等
                    //                        status = _CONNECT_STATUS_TIMEOUT;
                    //                    } else {
                    //                        status = _CONNECT_STATUS_FAILED;
                    //                        break;
                    //                    }
                    
                    if (error != 0) {
                        status = _CONNECT_STATUS_FAILED;
                        break;
                    }
                    
                } else if (ret == -1) {     // 返回-1， 有错误发生，错误原因存在于errno
                    status = _CONNECT_STATUS_FAILED;
                    break;
                } else {                     // 成功，返回描述词状态已改变的个数
                    status = _CONNECT_STATUS_SUCEESS;
                    break;
                }
                
                count++;
                if (count >= timeout_second) {
                    status = _CONNECT_STATUS_TIMEOUT;
                    break;
                }
            }
            
        }else{
            status = _CONNECT_STATUS_FAILED;
        }
        
    } else {
        status = _CONNECT_STATUS_SUCEESS;
    }
    
    fcntl((_INT)fd, F_SETFL, flags);
    
    return status;
}


_BOOL is_connected(_INT fd)
{
    if (fd == 0) {
        return _FALSE;
    }
    
    struct sockaddr remoteAddr;
    socklen_t socketLen = sizeof(remoteAddr);
    if (getpeername((int)fd, &remoteAddr, &socketLen) < 0)
    {
        return _FALSE;
    }
    return _TRUE;
}

_LONG send_data(_INT fd, _CONST _CHAR *binaryData,_LONG dataLength)
{
#ifdef MY_DEBUG
    _log("start send data!");
#endif
    
    _LONG ret = send((int)fd, binaryData, dataLength, 0);
#ifdef MY_DEBUG
    _log("send data compelete!");
#endif
    return ret;
    
}

_LONG receive_data(_INT fd, _CONST _CHAR *binaryData, _LONG &dataLength)
{
    //    scope_lock<ICMutex> lk(m_mutex);
    _LONG ret = recv((int)fd,(void*)binaryData, dataLength, 0);
    return ret;
}


_BOOL disconnect_socket(_INT fd)
{
    
    if (fd > 0 && close((int)fd) < 0) {
        return _FALSE;
    }
    return _TRUE;
}

