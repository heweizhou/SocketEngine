//
//  EngineInterface.m
//  PanoramaSDKTest
//
//  Created by YangYanfei on 15/11/19.
//  Copyright © 2015年 周贺伟. All rights reserved.
//

#import <Foundation/Foundation.h>


#include "NObjcCallBack.h"
#include "NetWorkEngineTools.hpp"
#import "SocketDefines.h"
#import "IDataRequest.h"
#import "BaseMsgPackage.h"

NObjcCallBack::NObjcCallBack(std::string post_key)
{
    m_post_key = post_key;
}

/**
 *	@brief	连接断开
 *	@param 	socketFD  服务端关闭了socket连接
 */
void NObjcCallBack::socketDidClosed(int socketFD)
{
//
    @autoreleasepool {
        NSNotificationCenter *center = [NSNotificationCenter defaultCenter];
        [center postNotificationName:[NSString stringWithUTF8String:m_post_key.c_str()] object:nil userInfo:N_POST_PARAM(socketFD, _socketDidClosed, @0, 0, [NSNull null])];
    }
}

/**
 *	@brief	收到消息
 *	@param 	socketFD    socketfd
 *	@param 	data 	    数据指针
 *	@param 	dataLength 	接收数据的长度
 */
void NObjcCallBack::onReceiveData(int socketFD,const char *data, long dataLength)
{
    @autoreleasepool {
        NSNotificationCenter *center = [NSNotificationCenter defaultCenter];
        NSData* sdata = [NSData dataWithBytes:data length:dataLength];
        [center postNotificationName:[NSString stringWithUTF8String:m_post_key.c_str()] object:nil userInfo:N_POST_PARAM(socketFD, _onReceiveData, sdata, 0, [NSNull null])];
    }
}

/**
 *	@brief	发送成功
 *	@param tag  sendData返回的tag
 */
void NObjcCallBack::socketSendSuccessed(int socketFD, void* param)
{
    assert(param);
    n_package_param* p = (n_package_param*)param;
    
    id obj = nil;
    if (p->package_param) {
        obj = CFBridgingRelease((void*)p->package_param);
    }else{
        obj = [NSNull null];
    }
    
    @autoreleasepool {
        NSNotificationCenter *center = [NSNotificationCenter defaultCenter];
        [center postNotificationName:[NSString stringWithUTF8String:m_post_key.c_str()] object:nil userInfo:N_POST_PARAM(socketFD, _socketSendSuccessed, @0, 0, obj)];
    }
}

/**
 *	@brief	发送失败
 *	@param  socketFD 发送数据成功的socketFD
 *  @return
 */
void NObjcCallBack::socketSendSuccessedFailed(int socketFD, void* param)
{
    assert(param);
    n_package_param* p = (n_package_param*)param;
    
    id obj = nil;
    if (p->package_param) {
        obj = CFBridgingRelease((void*)p->package_param);
    }else{
        obj = [NSNull null];
    }
    
    @autoreleasepool {
        NSNotificationCenter *center = [NSNotificationCenter defaultCenter];
        [center postNotificationName:[NSString stringWithUTF8String:m_post_key.c_str()] object:nil userInfo:N_POST_PARAM(socketFD, _socketSendSuccessedFailed, @0, 0, obj)];
    }
}

void NObjcCallBack::socketSendPackageInvalid(int socketFD, void* param)
{
    assert(param);
    n_package_param* p = (n_package_param*)param;
    
    id obj = nil;
    if (p->package_param) {
        obj = CFBridgingRelease((void*)p->package_param);
    }else{
        obj = [NSNull null];
    }
    
    @autoreleasepool {
        NSNotificationCenter *center = [NSNotificationCenter defaultCenter];
        [center postNotificationName:[NSString stringWithUTF8String:m_post_key.c_str()] object:nil userInfo:N_POST_PARAM(socketFD, _socketSendPackageInvalid, @0, 0, obj)];
    }
}

/**
 *	@brief	Socket连接成功
 *	@param 	socketFD    连接成功的socketFD
 */
void NObjcCallBack::socketConnectSuccessed(int socketFD)
{
    @autoreleasepool {
        NSNotificationCenter *center = [NSNotificationCenter defaultCenter];
        [center postNotificationName:[NSString stringWithUTF8String:m_post_key.c_str()] object:nil userInfo:N_POST_PARAM(socketFD, _socketConnectSuccessed, @0, 0, [NSNull null])];
    }
}

/**
 *	@brief	Socket连接失败 (远端无响应)
 *	@param 	socketFD    连接失败的socketFD
 */
void NObjcCallBack::socketConnectFailed(int socketFD)
{
    @autoreleasepool {
        NSNotificationCenter *center = [NSNotificationCenter defaultCenter];
        [center postNotificationName:[NSString stringWithUTF8String:m_post_key.c_str()] object:nil userInfo:N_POST_PARAM(socketFD, _socketConnectFailed, @0, 0, [NSNull null])];
    }
}

/**
 *	@brief	Socket连接超时
 *	@param 	socketFD    socket连接超时
 */
void NObjcCallBack::socketConnectTimeout(int socketFD)
{
    @autoreleasepool {
        NSNotificationCenter *center = [NSNotificationCenter defaultCenter];
        [center postNotificationName:[NSString stringWithUTF8String:m_post_key.c_str()] object:nil userInfo:N_POST_PARAM(socketFD, _socketConnectTimeout, @0, 0, [NSNull null])];
    }
}

void NObjcCallBack::socketConnectUserBreak(int sokcetFD)
{
    @autoreleasepool {
        NSNotificationCenter *center = [NSNotificationCenter defaultCenter];
        [center postNotificationName:[NSString stringWithUTF8String:m_post_key.c_str()] object:nil userInfo:N_POST_PARAM(sokcetFD, _socketConnectUserBreak, @0, 0, [NSNull null])];
    }
}

/**
 *	@brief	Socket连接错误(端口被占用,ip不对)
 *	@param 	socketFD    socket连接超时
 */
void NObjcCallBack::sockeErrorCallBack(int socketFD,SocketError error)
{
    @autoreleasepool {
        NSNotificationCenter *center = [NSNotificationCenter defaultCenter];
        [center postNotificationName:[NSString stringWithUTF8String:m_post_key.c_str()] object:nil userInfo:N_POST_PARAM(socketFD, _sockeErrorCallBack, @0, error, [NSNull null])];
    }
}

bool NObjcCallBack::socketSendRequestData(void* param, long& handler)
{
    assert(param);
    
    n_package_param* p = (n_package_param*)param;
    BaseMsgPackage* obj = nil;
    if (p->package_param) {
        
        obj = CFBridgingRelease((void*)p->package_param);
        id<IDataRequest> bean = obj.msg_param;
        
        @try {
            if ([bean conformsToProtocol:@protocol(IDataRequest)]) {
                @autoreleasepool {
                    NSData* data = [bean generateBinaryData];
                    handler = CNetWorkEngineTools::CreateShareMem([data bytes], [data length]);
                }
            }
        } @catch (NSException *exception) {
            handler = 0;
        }
        
        CFBridgingRetain(obj);
        if (handler == 0) {
            return NO;
        }
        
        return YES;
    }
    return NO;
    
}
