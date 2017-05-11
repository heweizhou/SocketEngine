//
//  FSNetEngineConstant.h
//  NetworkEngine
//
//  Created by YangYanfei on 15/11/20.
//  Copyright © 2015年 周贺伟. All rights reserved.
//

#ifndef FSNetEngineConstant_h
#define FSNetEngineConstant_h


/**
 *	@brief	创建socket的domain(指定使用何种地址类型) 默认创建使用AF_INET,用于createSocket(int domain, int protocal)的domain参数输入
 */
typedef enum{
    AF_Inet        = 2,    //AF_INET Ipv4网络协议。
    AF_Inet6       = 30    //AF_INET6 Ipv6网络协议。    其中AF 表示ADDRESS FAMILY 地址族
}SocketDomain;


/**
 *	@brief	创建socket的protocol（指定数据传输协议）用于createSocket(int domain, int protocal)的protocol参数输入
    AF_INET Ipv4网络协议。
    AF_INET6 Ipv6网络协议。

 */
typedef enum{
   Socket_Stream               = 1,          /* stream socket */
   Socket_Datagram             = 2,          /* datagram socket */
   Socket_raw                  = 3           /* raw-protocol interface */
    
}SocketProtocal;


/**
 *	@brief	Socket错误码
 */
typedef enum{
    SocketBadConfigError          = -100001,         //Invalid configuration（创建socket失败）
    SocketConnectRefuesd          = -100002,         //socket连接被拒绝
    SocketConnectAddressTavail    = -100003,         //远程地址无效。通常处理方法是选择新的地址，并重新连接或提示用户，关闭套接字并终止程序。
    SocketOtherError              = -100004          //其他错误
}SocketError;



#endif /* FaciShareNetEngineConstant_h */
