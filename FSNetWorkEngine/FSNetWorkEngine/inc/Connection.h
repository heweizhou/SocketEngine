//
//  Connection.h
//  FSNetWorkEngine
//
//  Created by 周贺伟 on 16/5/19.
//  Copyright © 2016年 周贺伟. All rights reserved.
//

#import <Foundation/Foundation.h>
//
#import "IDataRequest.h"

typedef NS_ENUM(NSUInteger, dominType) {
    INet,
    Local
};

@class Connection;

@protocol ConnectionDelegate <NSObject>

- (void)beforeConnection:(Connection*)connect;
- (void)afterConnection:(Connection*)connect;

@end

@protocol ConnectionStatusDelegate <NSObject>

- (void)socketDidClosed:(Connection*)connect;
- (void)socketConnectSuccessed:(Connection*)connect;
- (void)socketConnectFailed:(Connection*)connect;
- (void)socketConnectTimeout:(Connection*)connect;
- (void)socketConnectUserBreak:(Connection*)connect;
- (void)sockeErrorCallBack:(Connection*)connect errorCode:(int)code;

@end

@protocol SendStatusDeletate <NSObject>

- (void)sendSuccess:(Connection*)connect bean:(id<IDataRequest>)bean;
- (void)sendFailed:(Connection*)connect bean:(id<IDataRequest>)bean;
- (void)sendPackageInvalid:(Connection*)connect bean:(id<IDataRequest>)bean;

@end


@protocol DataArrivalDelegate <NSObject>

- (void)onDataArrival:(Connection*)connect data:(NSData*)buffer;

@end

@interface Connection : NSObject

@property (nonatomic, weak) id<ConnectionDelegate>          connectDelegate;
@property (nonatomic, weak) id<ConnectionStatusDelegate>    connectStatusDelegate;
@property (nonatomic, weak) id<DataArrivalDelegate>         dataDelegate;
@property (nonatomic, weak) id<SendStatusDeletate>          sendStatueDelegate;

- (id)initWithIp:(NSString *)ip port:(int)port timeout:(int)timeout type:(dominType)domin;

- (void)resetHostInfo:(NSString *)ip port:(int)port type:(dominType)domin;
- (void)resetTimeout:(int)timeout;

- (void)startConnect;
- (void)disConnect;
- (void)reConnect;

- (void)send:(id<IDataRequest>)bean HighPriority:(BOOL)HP;
- (void)cancelTask:(id<IDataRequest>)bean;
- (void)cancelAllTask;

@end
