//
//  ConnectionHub.m
//  NFCPSyncEngine
//
//  Created by 周贺伟 on 2017/5/4.
//  Copyright © 2017年 周贺伟. All rights reserved.
//

#import "ConnectionHub.h"
#import "Connection.h"
#import "TaskBean.h"

@interface ConnectionHub () <ConnectionDelegate, ConnectionStatusDelegate, DataArrivalDelegate, SendStatusDeletate>
{
    Connection*         _conn;
    int                 _timeout;
    
    NSMutableArray*     _conns;
    int                 _faildConnCount;
}

@end

@implementation ConnectionHub

- (id)initWithHosts:(NSArray*)hosts timeout:(int)timeout;
{
    self = [super init];
    if (self) {
        _timeout = timeout;
        _conn = nil;
        _conns = [[NSMutableArray alloc] init];
        
        _faildConnCount = 0;
        [self createConnections:hosts];
    }
    return self;
}

- (void)dealloc
{
    [self disConnect];
}

#pragma --mark private functions

-(void)createConnections:(NSArray*)hosts
{
    if (!hosts) {
        return ;
    }
    
    @synchronized (_conns) {
        [_conns removeAllObjects];
    }

    [hosts enumerateObjectsUsingBlock:^(id  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
        NSString* host = (NSString*)obj;
        NSArray* info = [host componentsSeparatedByString:@":"];
        
        if ([info count] == 2) {
            
            Connection* connection = [[Connection alloc] initWithIp:info[0] port:[info[1] intValue] timeout:_timeout type:IPV4];
            connection.dataDelegate = self;
            connection.connectDelegate = self;
            connection.sendStatueDelegate = self;
            connection.connectStatusDelegate = self;
            
            @synchronized (_conns) {
                [_conns addObject:connection];
            }
        }
    }];
}

#pragma --mark public functions
- (void)setHosts:(NSArray*)hosts
{
    [self createConnections:hosts];
}

- (void)setTiemout:(int)timeout;
{
    _timeout = timeout;
    
    if ([_conns count] <= 0) {
        return ;
    }
    
    @synchronized (_conns) {
        [_conns enumerateObjectsUsingBlock:^(id  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
            [(Connection*)obj resetTimeout:timeout];
        }];
    }
}
- (void)startConnect
{
    if (_delegate) {
        if ([_delegate respondsToSelector:@selector(beforeConnection)]) {
            [_delegate beforeConnection];
        }
    }
    
    _faildConnCount = 0;
    
    @synchronized (_conns) {
        [_conns enumerateObjectsUsingBlock:^(id  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
            [(Connection*)obj startConnect];
        }];
    }
}

- (void)disConnect
{
    if (_conn) {
        @synchronized (_conn) {
            [_conn disConnect];
            _conn = nil;
        }
    }
}

- (void)reConnect
{
    [self disConnect];
    [self startConnect];
}

- (BOOL)send:(id<IDataRequest>)bean HighPriority:(BOOL)HP
{
    if (_conn) {
        @synchronized (_conn) {
            [_conn send:bean HighPriority:HP];
            return YES;
        }
    }
    return NO;
}

- (BOOL)cancelTask:(id<IDataRequest>)bean
{
    if (_conn) {
        @synchronized (_conn) {
            [_conn cancelTask:bean];
            return YES;
        }
    }
    return NO;
}

- (BOOL)cancelAllTask
{
    if (_conn) {
        @synchronized (_conn) {
            [_conn cancelAllTask];
            return YES;
        }
    }
    
    return NO;
}


#pragma --mark ConnectionDelegate delegate

- (void)beforeConnection:(Connection*)connect
{
    NSParameterAssert(connect);
}

- (void)afterConnection:(Connection*)connect
{
    NSParameterAssert(connect);
}


#pragma --mark ConnectionStatusDelegate delegate connection process
- (void)socketConnectSuccessed:(Connection*)connect
{
    @synchronized (_conn) {
        if (!_conn) {
            _conn = connect;
        } else {
            [connect disConnect];
            return ;
        }
    }
    
    if (_delegate) {
        if ([_delegate respondsToSelector:@selector(socketConnectSuccessed:)]) {
            [_delegate socketConnectSuccessed:connect];
        }
    }
}

- (void)socketConnectFailed:(Connection*)connect
{
    @synchronized (_conn) {
        if (_conn) {
            return ;
        }
    }
    
    _faildConnCount++;
    @synchronized (_conns) {
        if ([_conns count] > _faildConnCount) {
            return ;
        }
    }
    
    if (_delegate) {
        if ([_delegate respondsToSelector:@selector(socketConnectFailed:)]) {
            [_delegate socketConnectFailed:connect];
        }
    }
}

- (void)socketConnectTimeout:(Connection*)connect
{
    @synchronized (_conn) {
        if (_conn) {
            return ;
        }
    }
    
    _faildConnCount++;
    @synchronized (_conns) {
        if ([_conns count] > _faildConnCount) {
            return ;
        }
    }
    
    if (_delegate) {
        if ([_delegate respondsToSelector:@selector(socketConnectTimeout:)]) {
            [_delegate socketConnectTimeout:connect];
        }
    }
}

- (void)sockeErrorCallBack:(Connection*)connect errorCode:(int)code
{
    @synchronized (_conn) {
        if (_conn) {
            return ;
        }
    }
    
    _faildConnCount++;
    @synchronized (_conns) {
        if ([_conns count] > _faildConnCount) {
            return ;
        }
    }
    
    if (_delegate) {
        if ([_delegate respondsToSelector:@selector(sockeErrorCallBack:errorCode:)]) {
            [_delegate sockeErrorCallBack:connect errorCode:code];
        }
    }
}

#pragma --mark ConnectionStatusDelegate delegate event process
- (void)socketDidClosed:(Connection*)connect
{
    @synchronized (_conn) {
        if (_conn != connect) {
            return ;
        }
    }
    
    if (_delegate) {
        if ([_delegate respondsToSelector:@selector(socketDidClosed:)]) {
            [_delegate socketDidClosed:connect];
        }
    }
    
}

- (void)socketConnectUserBreak:(Connection *)connect
{
    @synchronized (_conn) {
        if (_conn != connect) {
            return ;
        }
    }

    if (_delegate) {
        if ([_delegate respondsToSelector:@selector(socketConnectUserBreak:)]) {
            [_delegate socketConnectUserBreak:connect];
        }
    }
}

#pragma --mark SendStatusDelegate delegate
- (void)sendSuccess:(Connection*)connect bean:(TaskBean*)bean
{
    @synchronized (_conn) {
        if (_conn != connect) {
            return ;
        }
    }
    
    if (_delegate) {
        if ([_delegate respondsToSelector:@selector(sendSuccess:)]) {
            [_delegate sendSuccess:bean];
        }
    }

}

- (void)sendFailed:(Connection*)connect bean:(TaskBean*)bean
{
    @synchronized (_conn) {
        if (_conn != connect) {
            return ;
        }
    }
    
    if (_delegate) {
        if ([_delegate respondsToSelector:@selector(sendFailed:)]) {
            [_delegate sendFailed:bean];
        }
    }
}

- (void)sendPackageInvalid:(Connection*)connect bean:(TaskBean*)bean
{
    @synchronized (_conn) {
        if (_conn != connect) {
            return ;
        }
    }
    
    if (_delegate) {
        if ([_delegate respondsToSelector:@selector(sendPackageInvalid:)]) {
            [_delegate sendPackageInvalid:bean];
        }
    }
}

#pragma --mark DataArrivalDelegate delegate

- (void)onDataArrival:(Connection*)connect data:(NSData*)buffer
{
    @synchronized (_conn) {
        if (_conn != connect) {
            return ;
        }
    }
    
    if (_delegate) {
        if ([_delegate respondsToSelector:@selector(onDataArrival:data:)]) {
            [_delegate onDataArrival:connect data:buffer];
        }
    }
}


@end
