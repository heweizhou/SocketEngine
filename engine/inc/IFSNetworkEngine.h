//
//  ISocketEngine.h
//  NetworkEngine
//
//  Created by YangYanfei on 15/11/11.
//  Copyright © 2015年 zhouhewei. All rights reserved.
//

#ifndef ISocketEngine_h
#define ISocketEngine_h
#include "IFSCallBack.h"

class  IFSNetworkEngine
{
public:
    /**
     @brief 创建socket通信fd
     @return 返回创建socket FD----socket创建失败返回SocketBadConfigError
     */
    virtual long createSocket()=0;
    
    
    /**
     @brief socket通信回调处理
     @param cb  设置IFSCallBack*类型的回调指针
     */
    virtual void setCallBack(IFSCallBack* cb ) = 0;
    
    
    /**
     @brief 设置ip地址和端口完成socket连接
     @param socketFD     已经创建的socket fd
     @param ipAddress    ip地址
     @param port         端口
     @return 返回值：0-成功 ,-1-失败, 1-超时
     */
    virtual int connectToHost(long socketFD, const char* ipAddress, const unsigned int port, long socketMsgKind, int timeout_sec)=0;
    
    /**
     @brief 设置ip地址和端口完成异步socket连接
     @param socketFD     已经创建的socket fd
     @param ipAddress    ip地址
     @param port         端口
     @return 通过回调状态确定返回的结果
     */
    virtual bool async_connectToHost(long socketFD, const char* ipAddress, const unsigned int port, long socketMsgKind, int timeout_sec) = 0;
    
    /**
     @brief 返回当前的socket是否connect
     @param socketFD     已经创建的socket fd
     @return true-链接， false-断开
     */
    virtual bool isConnected(long socketFD)=0;
    
    /**
     *	@brief	 发送数据
     *	@param  binaryData 发送的数据
     *  @param  socketFD   发送数据到socket
     *  @param  dataLength 发送数据包长度
     *  @return true-数据发送成功， false-数据发送失败
     */
    virtual long sendData(long socketFD, const char* binaryData, long dataLength) =0;
//    virtual bool sendData(long socketFD, const char* binaryData, long dataLength, void* param, unsigned long param_len) =0;
    
    /**
     *	@brief	 发送数据
     *	@param  binaryData 发送的数据
     *  @param  socketFD   发送数据到socket
     *  @param  dataLength 发送数据包长度
     *  @return 通过回调状态确定返回的结果
     */
    virtual bool async_sendData(long socketFD, void* param, unsigned long param_len, bool pri) = 0;
//    virtual bool async_sendData(long socketFD, const char* binaryData, long dataLength, void* param, unsigned long param_len, bool pri) = 0;
    
    /**
     *	@brief	 断开socket连接
     *  @param socketFD    销毁创建的socket fd
     *  @return true-关闭成功， false-关闭失败
     */
    virtual bool disconnectSocket(long socketFD) = 0;
    
public:
    virtual ~IFSNetworkEngine(){};
};

extern "C"
{
    /**
     @brief	获取socket引擎实例
     */
    IFSNetworkEngine* getEngineInstance();
    
    /**
     @brief 析构引擎
    */
    void destorySocketEngineInstance();
}

#endif
