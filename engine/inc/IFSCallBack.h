//
//  IFaciShareCallBack.h
//  NetworkEngine
//
//  Created by YangYanfei on 15/11/11.
//  Copyright © 2015年 周贺伟. All rights reserved.
//

#ifndef IFaciShareCallBack_h
#define IFaciShareCallBack_h

#include "FSNetEngineConstant.h"

class IFSCallBack
{
public:
    /**
     *	@brief	连接断开
     *	@param 	socketFD  服务端关闭了socket连接
     */
    virtual void socketDidClosed(int socketFD) = 0;
    
    /**
     *	@brief	收到消息
     *	@param 	socketFD    socketfd
     *	@param 	data 	    数据指针
     *	@param 	dataLength 	接收数据的长度
     */
     virtual void onReceiveData(int socketFD,const char *data, long dataLength)=0;
    
    /**
     *	@brief	发送成功
     *	@param tag  sendData返回的tag
     */
     virtual void socketSendSuccessed(int socketFD, void* param) =0;
    
    /**
     *	@brief	发送失败
     *	@param  socketFD 发送数据成功的socketFD
     *  @return
     */
    virtual void socketSendSuccessedFailed(int socketFD, void* param) =0;
    
    virtual void socketSendPackageInvalid(int socketFD, void* param) = 0;
    
    /**
     *	@brief	Socket连接成功
     *	@param 	socketFD    连接成功的socketFD
     */
    virtual void socketConnectSuccessed(int socketFD) = 0;
    
    /**
     *	@brief	Socket连接失败
     *	@param 	socketFD    连接失败的socketFD
     */
    virtual void socketConnectFailed(int socketFD) = 0;
    
    /**
     *	@brief	Socket连接超时
     *	@param 	socketFD    socket连接超时
     */
    virtual void socketConnectTimeout(int socketFD) = 0;
    
    virtual void socketConnectUserBreak(int sokcetFD) = 0;
    /**
     *	@brief	Socket连接超时
     *	@param 	socketFD    socket连接超时
     */
    virtual void sockeErrorCallBack(int socketFD, SocketError errorType) = 0;
    
    virtual bool socketSendRequestData(int socketFD, void* param, char** buffer, unsigned long& buffer_size) = 0;


    virtual ~IFSCallBack(){}
};

#endif /* IFaciShareCallBack_h */
