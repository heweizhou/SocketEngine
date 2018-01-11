//
//  SocketUtility.cpp
//  SocketEngine
//
//  Created by 周贺伟 on 2017/6/26.
//  Copyright © 2017年 周贺伟. All rights reserved.
//

#include "SocketUtility.hpp"
#include <ifaddrs.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <map>
#include <vector>

#include <resolv.h>
#include <dns.h>

#include <sys/sysctl.h>
#include <netinet/in.h>

#include <errno.h>

#include "SocketDefines.h"

_INT CSocketUtility::socketErrorCode(_INT errorCode)
{
    _INT retErrorCode =-1;
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
    
    return retErrorCode;
}




