//
//  TestSession.h
//  SocketDemoClient
//
//  Created by 周贺伟 on 2018/1/12.
//  Copyright © 2018年 zhouhewei. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Channel.h"

@interface TestSession : NSObject

- (instancetype)initWithHosts:(NSArray*)hosts timeout:(int)timeout channel:(Channel*)channel;

@end
