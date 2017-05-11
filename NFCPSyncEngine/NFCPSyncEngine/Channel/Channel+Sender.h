//
//  Channel+Sender.h
//  NFCPSyncEngine
//
//  Created by 周贺伟 on 2017/3/6.
//  Copyright © 2017年 fxiaoke. All rights reserved.
//

#ifndef Channel_Sender_h
#define Channel_Sender_h

@protocol ChannelSendDelegate <NSObject>

- (BOOL)beforeTaskSend:(Channel*)channel bean:(TaskBean*)bean;
- (BOOL)sendTask:(Channel*)channel bean:(TaskBean*)bean HighPriority:(BOOL)HP;
- (void)afterTaskSend:(Channel*)channel bean:(TaskBean*)bean;
- (BOOL)cancelTask:(Channel*)channel bean:(TaskBean*)bean;
@end

@interface Channel(Sender)
- (void)setSendDelegate:(id<ChannelSendDelegate>)sendDelegate;
@end


#endif /* Channel_Sender_h */
