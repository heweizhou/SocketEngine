//
//  TestClient.m
//  SocketDemoClient
//
//  Created by 周贺伟 on 2018/1/12.
//  Copyright © 2018年 zhouhewei. All rights reserved.
//

#import "TestClient.h"

#import "TestChannel.h"
#import "TestSession.h"

#import "TestTaskBean.h"
#import "TestResponseMsg.h"

@interface TestClient() <ChannelTaskDelegate>
{
    TestSession*        _session;
    TestChannel*        _channel;
}
@end

@implementation TestClient

- (instancetype)init
{
    self = [super init];
    if (self) {
        _channel = [[TestChannel alloc] init];
        _channel.taskDelegate = self;
        
        _session = [[TestSession alloc] initWithHosts:nil timeout:30 channel:_channel];
    }
    
    return self;
}

- (void)sendMessage:(NSString*)msg
{
    TestTaskBean* task = [[TestTaskBean alloc] initWithData:msg];
    [_channel sendWithBean:task HighPriority:NO];
}

#pragma --mark ChannelTaskDelegate

- (BOOL)onProcessMsg:(TaskBean *)bean channel:(Channel *)channel withMsg:(id)msg
{
    TestResponseMsg* tMsg = (TestResponseMsg*)msg;
    
    NSLog(@"%@", [tMsg getPackageContent]);
    return NO;
}

@end
