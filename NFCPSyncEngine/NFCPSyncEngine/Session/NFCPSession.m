    //
//  NFCPSession.m
//  NFCPSyncEngine
//
//  Created by 周贺伟 on 16/6/3.
//  Copyright © 2016年 周贺伟. All rights reserved.
//

#import "NFCPSession.h"
#import "ConnectionHub.h"
#import "NFCPQueueUtils.h"

#import "Channel+CallBack.h"
#import "Channel+Sender.h"
#import "TaskBean+Private.h"

#define CHANNEL_NOTIFY_QUEUE_NAME @"channel.notify.queue"

@interface NFCPSession ()<ConnectionHubDelegate, ChannelSendDelegate>
{
    NSMutableArray<Channel*>*   _dataChannels;
    Channel*                    _authChanel;
    ConnectionHub*              _connectionsHub;
}
@end

@implementation NFCPSession

-(instancetype)init
{
    return [self initWithHosts:nil timeout:30];
}

-(instancetype)initWithHosts:(NSArray*)hosts timeout:(int)timeout
{
    self = [super init];
    if (self != nil) {
        
        [self sessionEnvInit:timeout hosts:hosts];
    }
    return self;
}

#pragma --mark private initi function

-(void)sessionEnvInit:(int)timeout hosts:(NSArray*)hosts
{
    _connectionsHub = [[ConnectionHub alloc] initWithHosts:hosts timeout:timeout];
    _connectionsHub.delegate = self;
    
    [self DataChannelsInit];
    [self sessionInitAction];
}

-(void)DataChannelsInit
{
    _authChanel = [[Channel alloc] init];
    _dataChannels = [[NSMutableArray<Channel*> alloc] init];
}

-(void)registerChannel:(Channel*)channel
{
    @synchronized (_dataChannels) {
        if (![_dataChannels containsObject:channel]) {
            [_dataChannels addObject:channel];
        }
    }
    
    [self enableChannel:channel];
    
    if ([_connectionsHub isConnectionAvaliable]) {
        dispatch_async([[NFCPQueueUtils sharedInstance] getGlobalQueue:CHANNEL_NOTIFY_QUEUE_NAME], ^{
            [channel onChannelInit];
        });
    }
}

-(BOOL)unRegisterChannle:(Channel*)channel
{
    @synchronized (_dataChannels) {
        if ([_dataChannels containsObject:channel]) {
            [self disableChannel:channel];
            [_dataChannels removeObject:channel];
            return YES;
        }
        
        return NO;
    }
}


-(void)sessionInitAction
{
    if (_sessionStatusDelegate && [_sessionStatusDelegate respondsToSelector:@selector(onSessionStatusInit:)]) {
        [_sessionStatusDelegate onSessionStatusInit:self];
    }
}

-(BOOL)isHaveRemindTask
{
    __block BOOL ret = NO;
    @synchronized (_dataChannels) {
        [_dataChannels enumerateObjectsUsingBlock:^(Channel * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
            if ([obj isKindOfClass:[Channel class]]) {
                
                ret = [obj isHavePenddingTask];
                if (ret == YES) {
                    *stop = YES;
                }
            }
        }];
    }
    
    return ret;
}


-(void)dealloc
{
    [_connectionsHub disConnect];
}

#pragma --mark public interface

-(void)setHosts:(NSArray*)hosts
{
    [_connectionsHub setHosts:hosts];
}

-(void)setTimeout:(int)timeout
{
    [_connectionsHub setTiemout:timeout];
}

-(void)start
{
    [self resume];
}

-(void)stop
{
    [self pause];
    [self notifyChannelConnectionInvalid];
}

-(void)pause
{
    [_authChanel cancelAllTask];
    [_connectionsHub disConnect];
}

-(void)resume
{
    [_connectionsHub reConnect];
}

#pragma --mark channelsenddelegate impl

- (BOOL)beforeTaskSend:(Channel*)channel bean:(TaskBean*)bean
{
    if ([_connectionsHub isConnectionAvaliable]) {
        return YES;
    }
    
    
    if ([self.sessionStatusDelegate respondsToSelector:@selector(onSessionStatusNeedRebuild:)]) {
        [self disableChannels:_dataChannels];
        [self disableChannel:_authChanel];
        [self.sessionStatusDelegate onSessionStatusNeedRebuild:self];
    }
    
    return NO;
}

- (BOOL)sendTask:(Channel*)channel bean:(TaskBean*)bean HighPriority:(BOOL)HP
{
    if ([_sessionActionDelegate respondsToSelector:@selector(onSessionActionSendTask:bean:)]) {
        [_sessionActionDelegate onSessionActionSendTask:channel bean:bean];
    }
    
    return [_connectionsHub send:(id<IDataRequest>)bean HighPriority:HP];
}

- (void)afterTaskSend:(Channel*)channel bean:(TaskBean*)bean
{
}

- (BOOL)cancelTask:(Channel*)channel bean:(TaskBean*)bean
{
    return [_connectionsHub cancelTask:(id<IDataRequest>)bean];
}

#pragma --mark ConnectionDelegate delegate

- (void)beforeConnection
{
    [_authChanel cancelAllTask];
    
    if (_sessionStatusDelegate && [_sessionStatusDelegate respondsToSelector:@selector(onSessionStatusStart:)]) {
        [_sessionStatusDelegate onSessionStatusStart:self];
    }
}

#pragma --mark ConnectionStatusDelegate delegate

- (void)socketDidClosed:(Connection*)connect
{
    if (_sessionStatusDelegate && [_sessionStatusDelegate respondsToSelector:@selector(onSessionStatusCloesd:)]) {
        [_sessionStatusDelegate onSessionStatusCloesd:self];
    }
}

- (void)socketConnectSuccessed:(Connection*)connect
{
    if (![_sessionActionDelegate respondsToSelector:@selector(onSessionActionIsAuthNeeded:)]) {
        [self notifyChannelInitSync];
        if ([_sessionStatusDelegate respondsToSelector:@selector(onSessionStatusSuccessed:)]) {
            [_sessionStatusDelegate onSessionStatusSuccessed:self];
        }
        return ;
    }
    
    if (![_sessionActionDelegate respondsToSelector:@selector(onSessionActionAuthWithChannel:dataChannels:session:)]){
        assert(0);
        [self notifyChannelInitSync];
        if ([_sessionStatusDelegate respondsToSelector:@selector(onSessionStatusSuccessed:)]) {
            [_sessionStatusDelegate onSessionStatusSuccessed:self];
        }
        return ;
    }
    
    
    if ([_sessionActionDelegate onSessionActionIsAuthNeeded:self]){
        
        [self disableChannels:_dataChannels];
        [self enableChannel:_authChanel];
        [_sessionActionDelegate onSessionActionAuthWithChannel:_authChanel dataChannels:[_dataChannels copy] session:self];
    }
    
    if ([_sessionStatusDelegate respondsToSelector:@selector(onSessionStatusSuccessed:)]) {
        [_sessionStatusDelegate onSessionStatusSuccessed:self];
    }
    
}

- (void)socketConnectFailed:(Connection*)connect
{
    if (_sessionStatusDelegate && [_sessionStatusDelegate respondsToSelector:@selector(onSessionStatusFailed:)]) {
        [_sessionStatusDelegate onSessionStatusFailed:self];
    }
}

- (void)socketConnectTimeout:(Connection*)connect
{
    if (_sessionStatusDelegate && [_sessionStatusDelegate respondsToSelector:@selector(onSessionStatusTimeout:)]) {
        [_sessionStatusDelegate onSessionStatusTimeout:self];
    }
}

- (void)socketConnectUserBreak:(Connection *)connect
{
}

- (void)sockeErrorCallBack:(Connection*)connect errorCode:(int)code
{
    if (_sessionStatusDelegate && [_sessionStatusDelegate respondsToSelector:@selector(onSessionStatusError:session:)]) {
        [_sessionStatusDelegate onSessionStatusError:code session:self];
    }
}

#pragma --mark private function
-(void)notifyChannelInitSync
{
    @synchronized (_dataChannels) {
        [_dataChannels enumerateObjectsUsingBlock:^(Channel * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
            dispatch_async([[NFCPQueueUtils sharedInstance] getGlobalQueue:CHANNEL_NOTIFY_QUEUE_NAME], ^{
                [obj onChannelInit];
            });
        }];
    }
}

-(void)notifyChannelConnectionInvalid
{
    @synchronized (_dataChannels) {
        [_dataChannels enumerateObjectsUsingBlock:^(Channel * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
            dispatch_async([[NFCPQueueUtils sharedInstance] getGlobalQueue:CHANNEL_NOTIFY_QUEUE_NAME], ^{
                [obj onSyncConnectionInvalid];
                [obj cancelAllTask];
            });
        }];
    }
}

- (void)disableChannels:(NSArray<Channel*>*)channels
{
    @synchronized (channels) {
        [channels enumerateObjectsUsingBlock:^(Channel * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
            [self disableChannel:obj];
        }];
    }
}

- (void)enableChannels:(NSArray<Channel*>*)channels
{
    @synchronized (channels) {
        [channels enumerateObjectsUsingBlock:^(Channel * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
            [self enableChannel:obj];
        }];
    }
}

- (void)disableChannel:(Channel*)channel
{
    [channel setSendDelegate:nil];
}

- (void)enableChannel:(Channel*)channel
{
    [channel setSendDelegate:self];
}

#pragma --mark SendStatusDelegate delegate
- (void)sendSuccess:(TaskBean*)bean
{
    [[bean getChannel] sendSuccess:(id<IDataRequest>)bean];
}

- (void)sendFailed:(TaskBean*)bean
{
    [self sockeErrorCallBack:nil errorCode:-100004];
    [[bean getChannel] sendFailed:(id<IDataRequest>)bean];
}

- (void)sendPackageInvalid:(TaskBean*)bean
{
    [[bean getChannel] sendPackageInvalid:(id<IDataRequest>)bean];
}

#pragma --mark DataArrivalDelegate delegate

- (void)onDataArrival:(Connection*)connect data:(NSData*)buffer
{
    if ([_sessionActionDelegate respondsToSelector:@selector(onSessionActionResponseData:)]) {
        NSArray<NFCPResponseMessage*>* responseMsg = [_sessionActionDelegate onSessionActionResponseData:buffer];
        for (NFCPResponseMessage* msg in responseMsg) {
            [self ChannelDispather:msg];
        }
    }
}

- (void)ChannelDispather:(NFCPResponseMessage*)messsage
{
    int64_t tag = [messsage getMeesageTag];
    __block TaskBean* bean = nil;
    @synchronized (_dataChannels) {
        [_dataChannels enumerateObjectsUsingBlock:^(Channel * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
            bean = [obj getTaskRefByTag:tag];
            if (bean) {
                *stop = YES;
                [obj processMsg:messsage bean:bean];

            }
        }];
    }
    
    if (bean) {
        return ;
    }
    
    //认证通道
    bean = [_authChanel getTaskRefByTag:tag];
    if (bean) {
        [_authChanel processMsg:messsage bean:bean];
        return ;
    }
}

@end
