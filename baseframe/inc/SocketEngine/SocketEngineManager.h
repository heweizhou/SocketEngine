//
//  SocketEngineManager.hpp
//  SocketEngine
//
//  Created by YangYanfei on 15/11/16.
//  Copyright © 2015年 周贺伟. All rights reserved.
//

#ifndef SocketEngineManager_h
#define SocketEngineManager_h

#include <stdio.h>
#include "defines.h"

class SocketEngineManager
{
public:
    static _VOID start_recv_thread(_DataHubMessageKind kind);
    static _VOID stop_recv_thread();
    /**
     @brief 创建socket通信fd
     @param protocol 创建socket的协议类型。ex，AF_INET，AF_UNIX
     @param type     socket数据类型      ex,SOCK_STREAM,SOCK_DGRAM,SOCK_RAW
     @return 返回创建socket FD
     */
    static _HANDLE createSocket(_INT protocol, _INT type);
    
    
    /**
     @brief 设置ip地址和端口完成socket连接
     @param socketFD     已经创建的socket fd
     @param ipAddress    ip地址
     @param port         端口
     @return 返回socket连接的状态
     */
    static _INT connectToHost(_HANDLE socketFD, _CONST _CHAR* ipAddress, _CONST _UINT port, _CONST _INT timeout_sec);
    
    /**
     @brief 返回当前的socket是否connect
     @param socketFD     已经创建的socket fd
     @return true-链接， false-断开
     */
    static _BOOL isConnected(_HANDLE socketFD);
    
    /**
     *	@brief	 发送数据
     *	@param  binaryData 发送的数据
     *  @param  socketFD   发送数据到socket
     *  @param  dataLength 发送数据包长度
     *  @return true-返回senddata数据结果
     */
    static _LONG sendData(_HANDLE socketFD, _CONST _CHAR *binaryData, _LONG dataLength);
    
    
    /**
     *	@brief	 接收数据
     *  @param  socketFD   接收数据的socket fd
     *	@param  binaryData 发送的数据
     *  @param  dataLength 接收数据包长度
     *  @return true-数据发送成功， false-数据发送失败
     */
    static _LONG receiveData(_HANDLE socketFD, _CHAR *binaryData, _LONG dataLength);
    
    
    /**
     *	@brief	 断开socket连接
     *  @param socketFD    销毁创建的socket fd
     *  @return true-关闭成功， false-关闭失败
     */
    static _BOOL disconnectSocket(_HANDLE socketFD);
    
};
#endif /* SocketEngine_hpp */
