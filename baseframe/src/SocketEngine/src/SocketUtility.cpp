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
//#include <netinet/in.h>
#include <arpa/inet.h>
//#include <fcntl.h>
//#include <netinet/tcp.h>
//#include <unistd.h>
//#include <netdb.h>

_string formatIPV4Address(struct in_addr ipv4Addr){
    _string address = "";
    
    char dstStr[INET_ADDRSTRLEN];
    char srcStr[INET_ADDRSTRLEN];
    memcpy(srcStr, &ipv4Addr, sizeof(struct in_addr));
    if(inet_ntop(AF_INET, srcStr, dstStr, INET_ADDRSTRLEN) != NULL){
        address = dstStr;
    }
    
    return address;
}

_string formatIPV6Address(struct in6_addr ipv6Addr){
    _string address = "";
    char dstStr[INET6_ADDRSTRLEN];
    char srcStr[INET6_ADDRSTRLEN];
    memcpy(srcStr, &ipv6Addr, sizeof(struct in6_addr));
    if(inet_ntop(AF_INET6, srcStr, dstStr, INET6_ADDRSTRLEN) != NULL){
        //        address = [NSString stringWithUTF8String:dstStr];
        address = dstStr;
    }
    
    return address;
}



_BOOL CSocketUtility::isIpv4Net(){
    
    bool isIPV6 = true;
    _string deviceIPAddressStr = deviceIPAdress();
    _string::size_type idx = deviceIPAddressStr.find(":");
    if (idx != _string::npos) {
        isIPV6 = true;
    }else{
        isIPV6 = false;
    }
    return !isIPV6;
}


/*!
 * 获取当前设备ip地址
 */
_string CSocketUtility::deviceIPAdress()
{
    _string address = "";
    struct ifaddrs *interfaces = NULL;
    struct ifaddrs *temp_addr = NULL;
    int success = 0;
    
    success = getifaddrs(&interfaces);
    
    if (success == 0) {  // 0 表示获取成功
        
        temp_addr = interfaces;
        while (temp_addr != NULL) {
            //NSLog(@"ifa_name===%@",[NSString stringWithUTF8String:temp_addr->ifa_name]);
            // Check if interface is en0 which is the wifi connection on the iPhone
            _string ifanameStr(temp_addr->ifa_name);
            _string::size_type idx = ifanameStr.find("en0");
            _string::size_type idx2 = ifanameStr.find("pdp_ip0");
            if (idx != _string::npos || idx2 != _string::npos)
            {
                //如果是IPV4地址，直接转化
                if (temp_addr->ifa_addr->sa_family == AF_INET){
                    // Get NSString from C String
                    address = formatIPV4Address(((struct sockaddr_in *)temp_addr->ifa_addr)->sin_addr);
                }
                
                //如果是IPV6地址
                else if (temp_addr->ifa_addr->sa_family == AF_INET6){
                    address = formatIPV6Address(((struct sockaddr_in6 *)temp_addr->ifa_addr)->sin6_addr);
                    _string addressStr = address;
                    _string uppercase4Str = addressStr.substr(0,4);
                    
                    if (!(addressStr.empty()) && !(uppercase4Str == "fe80")) break;
                }
            }
            
            temp_addr = temp_addr->ifa_next;
        }
        
        freeifaddrs(interfaces);
    }
    
    //以FE80开始的地址是单播地址
    _string uppercase4Str = address.substr(0,4);
    if (!(address.empty()) && !(uppercase4Str == "fe80")) {
        return address;
    } else {
        return "127.0.0.1";
    }
}

