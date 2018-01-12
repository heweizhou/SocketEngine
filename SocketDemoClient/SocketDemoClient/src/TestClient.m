//
//  TestClient.m
//  SocketDemoClient
//
//  Created by 周贺伟 on 2018/1/12.
//  Copyright © 2018年 zhouhewei. All rights reserved.
//

#import "TestClient.h"

#import "NFCPResponseMessage.h"
#import "NFCPSession.h"
#import "Channel.h"
#import "TaskBean.h"

@interface TestClient() <NFCPSessionStatusDelegate, NFCPSessionActionDelegate, ChannelTaskDelegate>
{
    Channel*        _channel;
    NFCPSession*    _session;
}
@end

@implementation TestClient

- (instancetype)initWithHosts:(NSArray*)hosts timeout:(int)timeout
{
    self = [super init];
    if (self) {
        
        _channel = [[Channel alloc] init];
        _channel.taskDelegate = self;
        
        _session = [[NFCPSession alloc] initWithHosts:hosts timeout:timeout];
        _session.sessionActionDelegate = self;
        _session.sessionStatusDelegate = self;
        
        [_session registerChannel:_channel];
        
        [_session start];
    }
    return self;
}

- (void)sendMessage:(NSString*)msg
{
    
}

#pragma --mark ChannelTaskDelegate

- (BOOL)onProcessMsg:(TaskBean *)bean channel:(Channel *)channel withMsg:(id)msg
{
    return NO;
}

#pragma --mark NFCPSessionActionDelegate

-(BOOL)onSessionActionIsAuthNeeded:(NFCPSession*)session
{
    return NO;
}

-(void)onSessionActionAuthWithChannel:(Channel*)channel dataChannels:(NSArray<Channel*>*)dataChannels session:(NFCPSession*)session
{
    
}

-(void)onSessionActionSendTask:(Channel*)channel bean:(id)bean
{
    
}

-(NSArray<NFCPResponseMessage*>*)onSessionActionResponseData:(NSData*)data
{
    return nil;
}

-(BOOL)onSessionActionUnHandledMsg:(NFCPResponseMessage*)msg
{
    return YES;
}

#pragma --mark NFCPSessionStatusDelegate

-(void)onSessionStatusInit:(NFCPSession*)session
{
    
}

-(void)onSessionStatusStart:(NFCPSession*)session
{
    
}

-(void)onSessionStatusSuccessed:(NFCPSession*)session
{
    
}

-(void)onSessionStatusTimeout:(NFCPSession*)session
{
    
}

-(void)onSessionStatusCloesd:(NFCPSession*)session
{
    
}

-(void)onSessionStatusFailed:(NFCPSession*)session
{
    
}

-(void)onSessionStatusError:(int)errorCode session:(NFCPSession*)session
{
    
}

-(void)onSessionStatusNeedRebuild:(NFCPSession*)session
{
    
}

@end
