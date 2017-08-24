//
//  EngineInterface.h
//  PanoramaSDKTest
//
//  Created by YangYanfei on 15/11/19.
//  Copyright © 2015年 周贺伟. All rights reserved.
//

#ifndef EngineInterface_h
#define EngineInterface_h
#import "IFSCallBack.h"

#include <string>

class  NObjcCallBack: public IFSCallBack
{
public:
    /**
     *	@brief	连接断开
     *	@param 	socketFD  服务端关闭了socket连接
     */
    virtual void socketDidClosed(int socketFD);
    
    /**
     *	@brief	收到消息
     *	@param 	socketFD    socketfd
     *	@param 	data 	    数据指针
     *	@param 	dataLength 	接收数据的长度
     */
    virtual void onReceiveData(int socketFD,const char *data, long dataLength);
    
    /**
     *	@brief	发送成功
     *	@param tag  sendData返回的tag
     */
    virtual void socketSendSuccessed(int socketFD, void* param);
    
    /**
     *	@brief	发送失败
     *	@param  socketFD 发送数据成功的socketFD
     *  @return
     */
    virtual void socketSendSuccessedFailed(int socketFD, void* param);
    
    virtual void socketSendPackageInvalid(int socketFD, void* param);
    /**
     *	@brief	Socket连接成功
     *	@param 	socketFD    连接成功的socketFD
     */
    virtual void socketConnectSuccessed(int socketFD);
    
    /**
     *	@brief	Socket连接失败
     *	@param 	socketFD    连接失败的socketFD
     */
    virtual void socketConnectFailed(int socketFD);
    
    /**
     *	@brief	Socket连接超时
     *	@param 	socketFD    socket连接超时
     */
    virtual void socketConnectTimeout(int socketFD);
    
    virtual void socketConnectUserBreak(int sokcetFD);
    /**
     *	@brief	Socket连接超时
     *	@param 	socketFD    socket连接超时
     */
    virtual void sockeErrorCallBack(int socketFD,SocketError errorType);
    
    virtual bool socketSendRequestData(void* param, long& handler);
    
public:
    NObjcCallBack(std::string post_key);
private:
    std::string     m_post_key;
};
#endif /* EngineInterface_h */
