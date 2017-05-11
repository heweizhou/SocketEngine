//
//  ConnectionDataDispather.m
//  FSNetWorkEngine
//
//  Created by 周贺伟 on 2017/3/13.
//  Copyright © 2017年 周贺伟. All rights reserved.
//

#import "ConnectionDataDispather.h"
#import "FSFCPNetWork.h"
#import "IFSNetworkEngine.h"
#import "SocketDefines.h"
#import "NObjcCallBack.h"

#define  konReceiveDataFromSocketKey @"_ConnectionDataDispather_onReceiveDataFromSocket"

@interface ConnectionDataDispather()
{
    NSMutableDictionary*       _fd_conn;
    NObjcCallBack*             _call_back;
}
@end

@implementation ConnectionDataDispather

+ (ConnectionDataDispather *)getInstance
{
    static ConnectionDataDispather *instance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        instance = [[ConnectionDataDispather alloc] init];
    });
    
    return instance;
}

- (instancetype)init
{
    self = [super init];
    if (self) {
        _fd_conn = [[NSMutableDictionary alloc] init];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(onReceiveDataFromSocket:) name:konReceiveDataFromSocketKey object:nil];
        
        _call_back = new NObjcCallBack([konReceiveDataFromSocketKey UTF8String]);
        getEngineInstance()->setCallBack(_call_back);
    }
    return self;
}

- (void)add_connection:(Connection*)conn fd:(int)fd
{
    @synchronized (_fd_conn) {
        [_fd_conn setObject:conn forKey:@(fd)];
    }
}

- (void)remove_connection:(int)fd
{
    @synchronized (_fd_conn) {
        [_fd_conn removeObjectForKey:@(fd)];
    }
}

- (void)onReceiveDataFromSocket:(NSNotification *)notification
{
    NSDictionary *dict = notification.userInfo;
    int fd = [dict[N_SOCKET_FD] intValue];
    int kind = [dict[N_SOCKET_MSG_KIND] intValue];
    id param = dict[N_SOCKET_PARAM];
    NSData *data = dict[N_SOCKET_DATA_BUFFER];
    NFSSocketError errorCode = (NFSSocketError)[dict[N_SOCKET_ERROR] intValue];
    
    Connection<ConnectionAdapterCallBack>* conn = nil;
    @synchronized (_fd_conn) {
        conn = [_fd_conn objectForKey:@(fd)];
    }
    
    if ([conn respondsToSelector:@selector(onMessageArrival:param:data:errorCode:)]) {
        [conn onMessageArrival:(socket_msg_kind)kind param:param data:data errorCode:errorCode];
    }
}

@end
