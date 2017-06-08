    //
//  NFCPSession.m
//  NFCPSyncEngine
//
//  Created by Lin on 16/6/3.
//  Copyright © 2016年 周贺伟. All rights reserved.
//

#import "NFCPSession.h"
#import "ConnectionHub.h"
#import "TaskBean.h"
#import "IAuthDelegate.h"
#import "Channel+CallBack.h"
#import "Channel+Sender.h"
#import "TaskBean+Private.h"
#import "NFCPQueueUtils.h"

#define CHANNEL_NOTIFY_QUEUE_NAME @"channel.notify.queue"

@interface NFCPSession ()<IAuthDelegate, ConnectionHubDelegate, ChannelSendDelegate>
{
    NSMutableArray<Channel*>*   _dataChannels;
    
    BOOL                        _isAuthed;
    Channel*                    _authChanel;
    id<INFCPAuth>               _auth;
    
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
    return [self initWithHosts:hosts timeout:timeout auth:nil];
}

-(instancetype)initWithHosts:(NSArray*)hosts timeout:(int)timeout auth:(id<INFCPAuth>)auth
{
    self = [super init];
    if (self != nil) {
        
        [self sessionEnvInit:timeout hosts:hosts];
        [self setAuth:auth];
    }
    return self;
}

#pragma --mark private initi function

-(void)sessionEnvInit:(int)timeout hosts:(NSArray*)hosts
{
    _isAuthed = NO;
    
    _connectionsHub = [[ConnectionHub alloc] initWithHosts:hosts timeout:timeout];
    _connectionsHub.delegate = self;
    
    [self DataChannelsInit];
    [self sessionInitAction];
}

-(void)DataChannelsInit
{
    _authChanel = [[Channel alloc] init];
    _dataChannels = [[NSMutableArray<Channel*> alloc] init];
    [self registerChannel:_authChanel];
}

-(void)registerChannel:(Channel*)channel
{
    @synchronized (_dataChannels) {
        [channel setSendDelegate:self];
        [_dataChannels addObject:channel];
        if (_isAuthed) {
            dispatch_async([[NFCPQueueUtils sharedInstance] getGlobalQueue:CHANNEL_NOTIFY_QUEUE_NAME], ^{
                [channel onChannelInit];
            });
        }
    }
}

-(BOOL)unRegisterChannle:(Channel*)channel
{
    @synchronized (_dataChannels) {
        if ([_dataChannels containsObject:channel]) {
            [channel setSendDelegate:nil];
            [_dataChannels removeObject:channel];
            return YES;
        }
        
        return NO;
    }
}


-(void)sessionInitAction
{
    if (_sessionStatusDelegate && [_sessionStatusDelegate respondsToSelector:@selector(onSessionStatusInit)]) {
        [_sessionStatusDelegate onSessionStatusInit];
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

-(void)setAuth:(id<INFCPAuth>)auth
{
    if (auth == nil) {
        _auth = nil;
        return ;
    }
    
    [auth setAuthDelegate:self];
    _auth = auth;
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
    _isAuthed = NO;
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
    if (channel == _authChanel) {
        return YES;
    }
    
    if (_isAuthed) {
        return YES;
    }
    
    if (self.sessionStatusDelegate) {
        if ([self.sessionStatusDelegate respondsToSelector:@selector(onSessionConnectionNeedRebuild)]) {
            [self.sessionStatusDelegate onSessionConnectionNeedRebuild];
        }
    }
    
    return NO;
}

- (BOOL)sendTask:(Channel*)channel bean:(TaskBean*)bean HighPriority:(BOOL)HP
{
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
    
    if (_sessionStatusDelegate && [_sessionStatusDelegate respondsToSelector:@selector(onSessionStatusStart)]) {
        [_sessionStatusDelegate onSessionStatusStart];
    }

}

#pragma --mark ConnectionStatusDelegate delegate

- (void)socketDidClosed:(Connection*)connect
{
    _isAuthed = NO;
    
    if (_sessionStatusDelegate && [_sessionStatusDelegate respondsToSelector:@selector(onSessionStatusCloesd)]) {
        [_sessionStatusDelegate onSessionStatusCloesd];
    }
}

- (void)socketConnectSuccessed:(Connection*)connect
{
    if (_sessionStatusDelegate && [_sessionStatusDelegate respondsToSelector:@selector(onSessionStatusSuccessed)]) {
        [_sessionStatusDelegate onSessionStatusSuccessed];
    }
    
    if (_sessionStatusDelegate && [_sessionStatusDelegate respondsToSelector:@selector(onSessionStatusStartAuth)]) {
        [_sessionStatusDelegate onSessionStatusStartAuth];
    }
    
    if (_auth) {
        [_auth startAuthWithChannel:_authChanel];
    } else {
        _isAuthed = YES;
        [self notifyChannelInitSync];
    }
}

- (void)socketConnectFailed:(Connection*)connect
{
    if (_sessionStatusDelegate && [_sessionStatusDelegate respondsToSelector:@selector(onSessionStatusFailed)]) {
        [_sessionStatusDelegate onSessionStatusFailed];
    }
}

- (void)socketConnectTimeout:(Connection*)connect
{
    if (_sessionStatusDelegate && [_sessionStatusDelegate respondsToSelector:@selector(onSessionStatusTimeout)]) {
        [_sessionStatusDelegate onSessionStatusTimeout];
    }
}

- (void)socketConnectUserBreak:(Connection *)connect
{
}

- (void)sockeErrorCallBack:(Connection*)connect errorCode:(int)code
{
    if (_sessionStatusDelegate && [_sessionStatusDelegate respondsToSelector:@selector(onSessionStatusError:)]) {
        [_sessionStatusDelegate onSessionStatusError:code];
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

-(void)ResponseAbandoned:(Channel*)channel
{
    if (_sessionStatusDelegate && [_sessionStatusDelegate respondsToSelector:@selector(onResponseAbandoned:)]) {
        [_sessionStatusDelegate onResponseAbandoned:channel];
    }
}

#pragma --mark IAuthDelegate delegate

-(void)onLinkSetup:(Channel*) channel parameters:(NSDictionary*) parameters
{
    if (_sessionStatusDelegate && [_sessionStatusDelegate respondsToSelector:@selector(onSessionLinkSetup:parameters:)]) {
        [_sessionStatusDelegate onSessionLinkSetup:channel parameters:parameters];
    }
}

-(void)onLinkSetupSuccessed:(Channel*) channel parameters:(NSDictionary*) parameters
{
    if (_sessionStatusDelegate && [_sessionStatusDelegate respondsToSelector:@selector(onSessionLinkSetupSuccessed:parameters:)]) {
        [_sessionStatusDelegate onSessionLinkSetupSuccessed:channel parameters:parameters];
    }
}

-(void)onLinkSetupFailed:(Channel*) channel parameters:(NSDictionary*) parameters
{
    if (_sessionStatusDelegate && [_sessionStatusDelegate respondsToSelector:@selector(onSessionLinkSetupFailed:parameters:)]) {
        [_sessionStatusDelegate onSessionLinkSetupFailed:channel parameters:parameters];
    }
}

-(void)onLinkSetupTimeout:(Channel*) channel parameters:(NSDictionary*) parameters
{
    if (_sessionStatusDelegate && [_sessionStatusDelegate respondsToSelector:@selector(onSessionLinkSetupTimeout:parameters:)]) {
        [_sessionStatusDelegate onSessionLinkSetupTimeout:channel parameters:parameters];
    }
}

-(void)onAuthSetup:(Channel*) channel parameters:(NSDictionary*) parameters
{
    if (_sessionStatusDelegate && [_sessionStatusDelegate respondsToSelector:@selector(onSessionAuthSetup:parameters:)]) {
        [_sessionStatusDelegate onSessionAuthSetup:channel parameters:parameters];
    }
}

-(void)onAuthSuccessed:(Channel*) channel parameters:(NSDictionary*) parameters
{
    if (_sessionStatusDelegate && [_sessionStatusDelegate respondsToSelector:@selector(onSessionStatusAuthSuccessed:parameters:)]) {
        [_sessionStatusDelegate onSessionStatusAuthSuccessed:channel parameters:parameters];
    }
    
    _isAuthed = YES;
    
    [self notifyChannelInitSync];// ready to delete
}

-(void)onAuthFailed:(Channel*) channel parameters:(NSDictionary*) parameters
{
    if (_sessionStatusDelegate && [_sessionStatusDelegate respondsToSelector:@selector(onSessionStatusAuthFailed:parameters:)]) {
        [_sessionStatusDelegate onSessionStatusAuthFailed:channel parameters:parameters];
    }
}

-(void)onAuthTimeout:(Channel*) channel parameters:(NSDictionary*) parameters
{
    if (_sessionStatusDelegate && [_sessionStatusDelegate respondsToSelector:@selector(onSessionStatusAuthTimeout:parameters:)]) {
        [_sessionStatusDelegate onSessionStatusAuthTimeout:channel parameters:parameters];
    }
}

#pragma --mark SendStatusDelegate delegate

- (void)sendSuccess:(TaskBean*)bean
{
    [[bean getChannel] sendSuccess:(id<IDataRequest>)bean];
}

- (void)sendFailed:(TaskBean*)bean
{
    [self sockeErrorCallBack:nil errorCode:FSSocketOtherError];
    [[bean getChannel] sendFailed:(id<IDataRequest>)bean];
}

- (void)sendPackageInvalid:(TaskBean*)bean
{
    [[bean getChannel] sendPackageInvalid:(id<IDataRequest>)bean];
}

#pragma --mark DataArrivalDelegate delegate

- (void)onDataArrival:(Connection*)connect data:(NSData*)buffer
{
    //to do
    //dispath the message from socket to channel
    assert(0);
    /*
     - (BOOL)onProcessMsg:(TaskBean*)bean channel:(Channel*)channel withMsg:(id)msg
    */
}

@end
