//
//  NFCPSession.h
//  NFCPSyncEngine
//
//  Created by Lin on 16/6/3.
//  Copyright © 2016年 fxiaoke. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "INFCPAuth.h"
#import "Channel.h"

typedef NS_ENUM (NSInteger, NFCPSessionStatus) {
    
    //Session 状态
    NFCPSessionStatusInit,
    NFCPSessionStatusStart,
    NFCPSessionStatusSuccessed,
    NFCPSessionStatusTimeout,
    NFCPSessionStatusInvalid,
    NFCPSessionStatusCloesd,
    NFCPSessionStatusFailed,
    NFCPSessionStatusError,
    
    NFCPSessionStatusStartAuth,  //将要启动认证状态
    
    //认证第一步build link状态
    NFCPSessionStatusLinkSetup,
    NFCPSessionStatusLinkSetupSuccessed,
    NFCPSessionStatusLinkSetupFailed,
    NFCPSessionStatusLinkSetupTimeout,
    
    //认证状态
    NFCPSessionStatusAuthSetup,
    NFCPSessionStatusAuthSuccessed,
    NFCPSessionStatusAuthFailed,
    NFCPSessionStatusAuthTimeout
};



@protocol NFCPSessionStatusDelegate <NSObject>

@optional

//Connection status
-(void)onSessionStatusInit;
-(void)onSessionStatusStart;
-(void)onSessionStatusSuccessed;
-(void)onSessionStatusTimeout;
-(void)onSessionStatusCloesd;
-(void)onSessionStatusFailed;
-(void)onSessionStatusError:(int)errorCode;
-(void)onSessionStatusStartAuth;

//Build Link status
-(void)onSessionLinkSetup:(Channel*) channel parameters:(NSDictionary*) parameters;
-(void)onSessionLinkSetupSuccessed:(Channel*) channel parameters:(NSDictionary*) parameters;
-(void)onSessionLinkSetupFailed:(Channel*) channel parameters:(NSDictionary*) parameters;
-(void)onSessionLinkSetupTimeout:(Channel*) channel parameters:(NSDictionary*) parameters;


//Do auth status
-(void)onSessionAuthSetup:(Channel*) channel parameters:(NSDictionary*) parameters;
-(void)onSessionStatusAuthSuccessed:(Channel*) channel parameters:(NSDictionary*)parameters;
-(void)onSessionStatusAuthFailed:(Channel*) channel parameters:(NSDictionary*)parameters;
-(void)onSessionStatusAuthTimeout:(Channel*) channel parameters:(NSDictionary*)parameters;

//logic status
-(void)onResponseAbandoned:(Channel*)channel;

//invaild status
-(void)onSessionConnectionNeedRebuild;

@end


@interface NFCPSession : NSObject
@property (nonatomic, weak) id<NFCPSessionStatusDelegate> sessionStatusDelegate;

-(instancetype)init;

-(instancetype)initWithHosts:(NSArray*)hosts timeout:(int)timeout;
-(instancetype)initWithHosts:(NSArray*)hosts timeout:(int)timeout auth:(id<INFCPAuth>)auth;

-(void)setHosts:(NSArray*)hosts;
-(void)setAuth:(id<INFCPAuth>)auth;
-(void)setTimeout:(int)timeout;

-(void)start;
-(void)stop;
-(void)pause;
-(void)resume;

-(void)registerChannel:(Channel*)channel;
-(BOOL)unRegisterChannle:(Channel*)channle;

-(BOOL)isHaveRemindTask;

@end
