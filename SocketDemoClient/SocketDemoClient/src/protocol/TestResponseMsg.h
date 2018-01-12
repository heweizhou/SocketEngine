//
//  TestResponseMsg.h
//  SocketDemoClient
//
//  Created by 周贺伟 on 2018/1/12.
//  Copyright © 2018年 zhouhewei. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "NFCPResponseMessage.h"

@interface TestResponseMsg : NFCPResponseMessage

- (instancetype)initWithRawData:(NSData*)data;
- (NSString*)getPackageContent;

@end
