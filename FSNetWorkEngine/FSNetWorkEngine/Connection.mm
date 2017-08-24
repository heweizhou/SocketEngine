//
//  Connection.m
//  FSNetWorkEngine
//
//  Created by 周贺伟 on 16/5/19.
//  Copyright © 2016年 zhouhewei. All rights reserved.
//

#import "Connection.h"
#import "SocketDefines.h"
#import "ConnectionDataDispather.h"
#import "BaseMsgPackage.h"
#include "IFSNetworkEngine.h"

#define  SOCKET_CONNECTION_TIMEOUT     60

#pragma -- mark Connection

@interface Connection()<ConnectionAdapterCallBack>

@property (nonatomic, copy) NSString*               ip;
@property (nonatomic, assign) int                   fd;
@property (nonatomic, assign) int                   port;
@property (nonatomic, assign) int                   timeout;
@end

@implementation Connection

-(id)init
{
    return [self initWithIp:nil port:0 timeout:30];
}

- (id)initWithIp:(NSString *)ip port:(int)port timeout:(int)timeout
{
    self = [super init];
    if (self) {
        self.fd = -1;
        self.port = port;
        self.ip = ip;
        [self initTimer:timeout];
    }
    return self;
}

- (void)resetHostInfo:(NSString *)ip port:(int)port
{
    self.ip = ip;
    self.port = port;
}

- (void)resetTimeout:(int)timeout
{
    [self initTimer:timeout];
}

-(void)initTimer:(int)interval
{
    //计数归零
    if (interval <=0) {
        self.timeout = SOCKET_CONNECTION_TIMEOUT;
    }
    else
    {
        self.timeout = interval;
    }
}

-(void)dealloc
{
    [self disConnect];
}

- (void)startConnect
{
    self.fd = (int)getEngineInstance()->createSocket();
    [self beforeConnection];
    [[ConnectionDataDispather getInstance] add_connection:self fd:self.fd];
    getEngineInstance()->async_connectToHost(self.fd, [self.ip UTF8String], self.port, 0, self.timeout);
}

- (void)disConnect
{
    [[ConnectionDataDispather getInstance] remove_connection:self.fd];
    getEngineInstance()->disconnectSocket(self.fd);
    self.fd = -1;
}

- (void)reConnect
{
    [self disConnect];
    [self startConnect];
}

-(void)beforeConnection
{
    if (self.connectDelegate) {
        if ([self.connectDelegate respondsToSelector:@selector(beforeConnection:)]) {
            [self.connectDelegate beforeConnection:self];
        }
    }
}

-(void)afterConnection
{
    if (self.connectDelegate) {
        if ([self.connectDelegate respondsToSelector:@selector(afterConnection:)]) {
            [self.connectDelegate afterConnection:self];
        }
    }
}

- (void)send:(id<IDataRequest>)bean HighPriority:(BOOL)HP;
{
    BaseMsgPackage* package = [[BaseMsgPackage alloc] initWithMsgInfo:self.fd param:bean pri:HP];
    n_package_param param_package = {0, (long)CFBridgingRetain(package)};
    getEngineInstance()->async_sendData(self.fd, &param_package, sizeof(n_package_param), HP);
}

- (void)cancelTask:(id<IDataRequest>)bean
{
}

- (void)cancelAllTask
{
}

#pragma --mark call back function

- (NSocketSendStatus)mapping:(socket_msg_kind)kind
{
    switch (kind) {
        case _socketSendSuccessed:
            return NSocketSendSuccessed;
            
        case _socketSendSuccessedFailed:
            return NSocketSendSuccessedFailed;
        default:
            return NReserved;
    }
}

- (void)onMessageArrival:(socket_msg_kind)msgKind param:(id)param data:(NSData *)buffer errorCode:(NFSSocketError)code
{
    
    switch (msgKind) {
        case _socketDidClosed:
        {
            if (self.connectStatusDelegate) {
                if ([self.connectStatusDelegate respondsToSelector:@selector(socketDidClosed:)]) {
                    [self.connectStatusDelegate socketDidClosed:self];
                }
            }
        }
            break;
        case _socketConnectSuccessed:
        {
            [self afterConnection];
            if (self.connectStatusDelegate) {
                if ([self.connectStatusDelegate respondsToSelector:@selector(socketConnectSuccessed:)]) {
                    [self.connectStatusDelegate socketConnectSuccessed:self];
                }
            }
        }
            break;
        case _socketConnectFailed:
        {
            if (self.connectStatusDelegate) {
                if ([self.connectStatusDelegate respondsToSelector:@selector(socketConnectFailed:)]) {
                    [self.connectStatusDelegate socketConnectFailed:self];
                }
            }
        }
            break;
        case _socketConnectTimeout:
        {
            if (self.connectStatusDelegate) {
                if ([self.connectStatusDelegate respondsToSelector:@selector(socketConnectTimeout:)]) {
                    [self.connectStatusDelegate socketConnectTimeout:self];
                }
            }
        }
            break;
            
        case _socketConnectUserBreak:
        {
            if (self.connectStatusDelegate) {
                if ([self.connectStatusDelegate respondsToSelector:@selector(socketConnectUserBreak:)]) {
                    [self.connectStatusDelegate socketConnectUserBreak:self];
                }
            }
        }
            break;
        case _sockeErrorCallBack:
        {
            if (self.connectStatusDelegate) {
                if ([self.connectStatusDelegate respondsToSelector:@selector(sockeErrorCallBack:errorCode:)]) {
                    [self.connectStatusDelegate sockeErrorCallBack:self errorCode:code];
                }
            }
        }
            break;
            
        case _socketSendSuccessed:
        {
            if (self.sendStatueDelegate) {
                if ([self.sendStatueDelegate respondsToSelector:@selector(sendSuccess:bean:)]) {
                    BaseMsgPackage* package = (BaseMsgPackage*)param;
                    [self.sendStatueDelegate sendSuccess:self bean:package.msg_param];
                }
            }
            break;
        }
        case _socketSendSuccessedFailed:
        {
            if (self.sendStatueDelegate) {
                if ([self.sendStatueDelegate respondsToSelector:@selector(sendSuccess:bean:)]) {
                    BaseMsgPackage* package = (BaseMsgPackage*)param;
                    [self.sendStatueDelegate sendFailed:self bean:package.msg_param];
                }
            }
            break;
        }
        case _socketSendPackageInvalid:
        {
            if (self.sendStatueDelegate) {
                if ([self.sendStatueDelegate respondsToSelector:@selector(sendSuccess:bean:)]) {
                    BaseMsgPackage* package = (BaseMsgPackage*)param;
                    [self.sendStatueDelegate sendPackageInvalid:self bean:package.msg_param];
                }
            }
            break;
        }
            

        case _onReceiveData:
        {
            if (self.dataDelegate) {
                if ([self.dataDelegate respondsToSelector:@selector(onDataArrival:data:)]) {
                    [self.dataDelegate onDataArrival:self data:buffer];
                }
            }
        }
            break;
        default:
        {
        }
            break;
    }
}

- (NSString*)description
{
    return [NSString stringWithFormat:@"ip= %s; port = %d", [_ip UTF8String], _port];
}

@end
