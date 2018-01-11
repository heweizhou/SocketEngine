//
//  NFCPSession.h
//  NFCPSyncEngine
//
//  Created by 周贺伟 on 16/6/3.
//  Copyright © 2016年 周贺伟. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Channel.h"
#import "NFCPResponseMessage.h"

@class NFCPSession;

@protocol NFCPSessionStatusDelegate <NSObject>
@optional
//Connection status
-(void)onSessionStatusInit:(NFCPSession*)session;
-(void)onSessionStatusStart:(NFCPSession*)session;
-(void)onSessionStatusSuccessed:(NFCPSession*)session;
-(void)onSessionStatusTimeout:(NFCPSession*)session;
-(void)onSessionStatusCloesd:(NFCPSession*)session;
-(void)onSessionStatusFailed:(NFCPSession*)session;
-(void)onSessionStatusError:(int)errorCode session:(NFCPSession*)session;
-(void)onSessionStatusNeedRebuild:(NFCPSession*)session;
//-(void)onSessionStatusResponseAbandoned:(NFCPSession*)session;
@end



@protocol NFCPSessionActionDelegate <NSObject>

@optional
-(BOOL)onSessionActionIsAuthNeeded:(NFCPSession*)session;
-(void)onSessionActionAuthWithChannel:(Channel*)channel dataChannels:(NSArray<Channel*>*)dataChannels session:(NFCPSession*)session;
-(void)onSessionActionSendTask:(Channel*)channel bean:(id)bean;

@required
-(NSArray<NFCPResponseMessage*>*)onSessionActionResponseData:(NSData*)data;
-(BOOL)onSessionActionUnHandledMsg:(NFCPResponseMessage*)msg;
@end


@interface NFCPSession : NSObject
@property (nonatomic, weak) id<NFCPSessionStatusDelegate> sessionStatusDelegate;
@property (nonatomic, weak) id<NFCPSessionActionDelegate> sessionActionDelegate;

-(instancetype)init;
-(instancetype)initWithHosts:(NSArray*)hosts timeout:(int)timeout;

-(void)setHosts:(NSArray*)hosts;
-(void)setTimeout:(int)timeout;

-(void)start;
-(void)stop;
-(void)pause;
-(void)resume;

-(void)registerChannel:(Channel*)channel;
-(BOOL)unRegisterChannle:(Channel*)channle;

-(BOOL)isHaveRemindTask;

@end
