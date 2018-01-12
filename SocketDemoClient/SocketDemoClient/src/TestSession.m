//
//  TestSession.m
//  SocketDemoClient
//
//  Created by 周贺伟 on 2018/1/12.
//  Copyright © 2018年 zhouhewei. All rights reserved.
//

#import "TestSession.h"

#import "NFCPSession.h"
#import "Channel.h"
#import "TaskBean.h"

#import "TestTaskBean.h"
#import "TestResponseMsg.h"

@interface TestSession() <NFCPSessionStatusDelegate, NFCPSessionActionDelegate>
{
    NFCPSession*    _session;
}
@end

@implementation TestSession

- (instancetype)initWithHosts:(NSArray*)hosts timeout:(int)timeout channel:(Channel*)channel
{
    self = [super init];
    if (self) {
        
        _session = [[NFCPSession alloc] initWithHosts:hosts timeout:timeout];
        _session.sessionActionDelegate = self;
        _session.sessionStatusDelegate = self;
        
        [_session registerChannel:channel];
        
        [_session start];
    }
    return self;
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
    TestResponseMsg* msg = [[TestResponseMsg alloc] initWithRawData:data];
    return @[msg];
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
