//
//  ConnectionDataDispather.h
//  FSNetWorkEngine
//
//  Created by 周贺伟 on 2017/3/13.
//  Copyright © 2017年 fxiaoke. All rights reserved.
//

#import <Foundation/Foundation.h>
#include "Connection.h"
#import "NFCPSyncEnums.h"
#import "SocketDefines.h"

@protocol ConnectionAdapterCallBack <NSObject>
- (void)onMessageArrival:(socket_msg_kind)msgKind param:(id)param data:(NSData *)buffer errorCode:(NFSSocketError)code;
@end

@interface ConnectionDataDispather : NSObject
+ (ConnectionDataDispather *)getInstance;
- (void)add_connection:(Connection<ConnectionAdapterCallBack>*)conn fd:(int)fd;
- (void)remove_connection:(int)fd;
@end
