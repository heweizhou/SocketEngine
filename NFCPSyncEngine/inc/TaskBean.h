//
//  TaskBean.h
//  FSNetWorkEngine
//
//  Created by 周贺伟 on 16/5/18.
//  Copyright © 2016年 fxiaoke. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface TaskBean : NSObject
@property(nonatomic, assign)            int                timeout;
@property(nonatomic, strong)            NSMutableData*     data;
@property(nonatomic, strong)            id                 userData;

-(instancetype)init;
-(instancetype)initWithTag:(int64_t)tag;
-(NSData*)createPackage;

@end
