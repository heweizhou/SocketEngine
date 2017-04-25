//
//  BaseMsgPackage.m
//  FSNetWorkEngine
//
//  Created by 周贺伟 on 16/7/7.
//  Copyright © 2016年 fxiaoke. All rights reserved.
//

#import "BaseMsgPackage.h"

@implementation BaseMsgPackage

-(instancetype)initWithMsgInfo:(int)fd param:(id)param pri:(BOOL)isHighPri
{
    self = [super init];
    if (self) {
        self.fd = fd;
        self.msg_param = param;
        self.isHighPri = isHighPri;
    }
    return self;
}

@end
