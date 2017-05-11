//
//  ConnectionHub.h
//  NFCPSyncEngine
//
//  Created by 周贺伟 on 2017/5/4.
//  Copyright © 2017年 周贺伟. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "IDataRequest.h"
#import "Connection.h"
#import "TaskBean.h"


@protocol ConnectionHubDelegate <NSObject>

- (void)beforeConnection;

- (void)socketDidClosed:(Connection*)connect;
- (void)socketConnectSuccessed:(Connection*)connect;
- (void)socketConnectFailed:(Connection*)connect;
- (void)socketConnectTimeout:(Connection*)connect;
- (void)socketConnectUserBreak:(Connection *)connect;
- (void)sockeErrorCallBack:(Connection*)connect errorCode:(int)code;

- (void)sendSuccess:(TaskBean*)bean;
- (void)sendFailed:(TaskBean*)bean;
- (void)sendPackageInvalid:(TaskBean*)bean;

- (void)onDataArrival:(Connection*)connect data:(NSData*)buffer;

@end

@interface ConnectionHub : NSObject

@property(nonatomic, weak) id<ConnectionHubDelegate> delegate;

- (id)initWithHosts:(NSArray*)hosts timeout:(int)timeout;
- (void)startConnect;
- (void)disConnect;
- (void)reConnect;

- (BOOL)send:(id<IDataRequest>)bean HighPriority:(BOOL)HP;
- (BOOL)cancelTask:(id<IDataRequest>)bean;
- (BOOL)cancelAllTask;

- (void)setHosts:(NSArray*)hosts;
- (void)setTiemout:(int)timeout;

@end
