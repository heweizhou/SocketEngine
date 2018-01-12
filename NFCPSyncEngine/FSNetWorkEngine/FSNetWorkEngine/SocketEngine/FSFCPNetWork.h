//
//  FCPNetWork.h
//  FCPNetWork
//
//  Created by 周贺伟 on 15/12/22.
//  Copyright © 2015年 周贺伟. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "SocketDefines.h"

@interface FSFCPNetWork : NSObject

+ (FSFCPNetWork *)sharedNetWorkManager;

-(void)registerSokcet:(id)obj ip:(NSString*)ip port:(unsigned int)port timeout:(int)timeout msgKind:(long)kind selector:(SEL)selector connectMethodIsAsync:(BOOL)isAsync;
-(void)unRegisterSokcet:(id)obj msgKind:(long)kind;
-(BOOL)async_send:(long)msgKind buffer:(NSData*)data param:(id)param pri:(BOOL)isHighPri;

-(id)connectionIsExsit:(long)msgKind;
-(BOOL)isConnected:(id)connection;

@end
