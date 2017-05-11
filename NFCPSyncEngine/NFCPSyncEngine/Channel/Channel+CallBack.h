//
//  Channel+CallBack.h
//  NFCPSyncEngine
//
//  Created by 周贺伟 on 2017/3/3.
//  Copyright © 2017年 fxiaoke. All rights reserved.
//

#ifndef Channel_CallBack_h
#define Channel_CallBack_h

#import "TaskBean.h"

@interface Channel(CallBack)
- (void)sendSuccess:(TaskBean*)bean;
- (void)sendFailed:(TaskBean*)bean;
- (void)sendPackageInvalid:(TaskBean*)bean;
- (void)processMsg:(id)msg bean:(TaskBean*)bean;
- (TaskBean*)getTaskRefByTag:(int64_t)tag;

- (BOOL)isHavePenddingTask;
- (void)onChannelInit;
- (void)onSyncConnectionInvalid;
@end
#endif /* Channel_CallBack_h */
