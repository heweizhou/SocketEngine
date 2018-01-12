//
//  FSNetworkEngine.h
//  NetworkEngine
//
//  Created by 周贺伟 on 15/11/16.
//  Copyright © 2015年 fxiaoke. All rights reserved.
//

#ifndef FSNetworkEngine_h
#define FSNetworkEngine_h

#include "IFSNetworkEngine.h"
#include "DataRequest.h"
#include "SocketRecvManager.h"
#include "SocketHandlerMapper.hpp"

class FSNetworkEngine:public IFSNetworkEngine,IHubRequestBack
{
    //IHubRequestBack
private:
    virtual _VOID OnDataBack( _HANDLE memHandler, _PVOID reserve, PParamData param);
    virtual _VOID OnEvent(_UINT code, _PVOID reserve, PParamData param);
public:
    /**
     @brief 创建socket通信fd
     @return 返回创建socket FD(socket创建失败返回INNER_EVENT_SOCKET_CREATE_FAILED)
     */
    virtual _HANDLE createSocket();
    
    
    /**
     @brief socket通信回调处理
     @param cb  设置IFaciShareCallBack*类型的回调指针
     */
    virtual _VOID setCallBack(IFSCallBack* cb);
 
    /**
     @brief 设置ip地址和端口完成socket连接
     @param socketFD     已经创建的socket fd
     @param ipAddress    ip地址
     @param port         端口
     @return 返回创建socket FD
     */
    virtual _INT connectToHost( _HANDLE socketFD, _CONST _CHAR* ipAddress, _CONST _UINT port, _LONG socketMsgKind, _INT timeout_sec);
    
    /**
     @brief 设置ip地址和端口完成异步socket连接
     @param socketFD     已经创建的socket fd
     @param ipAddress    ip地址
     @param port         端口
     @return 返回创建socket FD
     */
    virtual _BOOL async_connectToHost( _HANDLE socketFD, _CONST _CHAR* ipAddress, _CONST _UINT port, _LONG socketMsgKind, _INT timeout_sec);
    
    /**
     @brief 返回当前的socket是否connect
     @param socketFD     已经创建的socket fd
     @return true-链接， false-断开
     */
    virtual _BOOL isConnected(_HANDLE socketFD);
    
    /**
     *	@brief	 发送数据
     *	@param  binaryData 发送的数据
     *  @param  socketFD   发送数据到socket
     *  @param  dataLength 发送数据包长度
     *  @return true-数据发送成功， false-数据发送失败
     */
    virtual _LONG sendData(_HANDLE socketFD, _CONST _CHAR *binaryData, _LONG dataLength);
    /**
     *	@brief	 发送数据
     *	@param  binaryData 发送的数据
     *  @param  socketFD   发送数据到socket
     *  @param  dataLength 发送数据包长度
     *  @return true-数据发送成功， false-数据发送失败
     */
    virtual _BOOL async_sendData(_HANDLE socketFD, _PVOID param, _ULONG param_len, _BOOL pri);
    
    /**
     *	@brief	 断开socket连接
     *  @param socketFD    销毁创建的socket fd
     *  @return true-关闭成功， false-关闭失败
     */
    virtual _BOOL disconnectSocket(_HANDLE socketFD);
    
public:
    FSNetworkEngine();
    virtual ~FSNetworkEngine();

private:
    SocketRecvManager*      m_socket_recv_mgr;
    CSocketHandlerMapper*   m_socket_handler_mapper;
    
    IFSCallBack*            m_faciShareCallBack;
};



#endif /* FSNetworkEngine_h */
