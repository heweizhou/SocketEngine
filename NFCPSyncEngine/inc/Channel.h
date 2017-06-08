//
//  Channel.h
//  FSNetWorkEngine
//
//  Created by 周贺伟 on 16/5/23.
//  Copyright © 2016年 周贺伟. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "TaskBean.h"
#import "NFCPDefines.h"

@class Channel;
@protocol ChannelTaskDelegate <NSObject>
@optional
- (void)sendSuccess:(TaskBean*)bean channel:(Channel*)channel;
- (void)sendFailed:(TaskBean*)bean channel:(Channel*)channel;
- (void)sendPackageInvalid:(TaskBean*)bean channel:(Channel*)channel;
- (void)onBeanTimeout:(Channel*)bean channel:(Channel*)channel;
- (void)onTaskCancel:(NSArray<TaskBean*>*)UnDonebeans;
- (void)onDataLinkInvalid:(Channel*)channel bean:(TaskBean*)bean;
- (BOOL)onProcessMsg:(TaskBean*)bean channel:(Channel*)channel withMsg:(id)msg;
@end


@protocol ChannelStatuesDelegate <NSObject>
@optional
- (BOOL)is_have_pedding_task;
- (void)on_channel_init;
- (void)on_connect_invalid;
- (void)on_timer_action;
@end

@interface Channel : NSObject

@property (nonatomic, weak) id<ChannelTaskDelegate>     taskDelegate;
@property (nonatomic, weak) id<ChannelStatuesDelegate>  statuesDelegate;

// init without channel
- (instancetype)init;

//send message
- (void)sendWithBean:(TaskBean*)bean HighPriority:(BOOL)HP;
- (void)cancelTask:(TaskBean*)bean;
- (void)cancelAllTask;

//function switch
-(BOOL)enableTaskManage;
-(BOOL)isDefalutChannel;

@end
