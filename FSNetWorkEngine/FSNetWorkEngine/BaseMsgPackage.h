//
//  BaseMsgPackage.h
//  FSNetWorkEngine
//
//  Created by 周贺伟 on 16/7/7.
//  Copyright © 2016年 fxiaoke. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface BaseMsgPackage : NSObject
@property (nonatomic, assign) int  fd;
@property (nonatomic, strong) id   msg_param;
@property (nonatomic, assign) BOOL isHighPri;

-(instancetype)initWithMsgInfo:(int)fd param:(id)param pri:(BOOL)isHighPri;
@end
