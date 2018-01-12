//
//  TaskBean.h
//  FSNetWorkEngine
//
//  Created by 周贺伟 on 16/5/18.
//  Copyright © 2016年 周贺伟. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface TaskBean : NSObject
@property(nonatomic, assign, readonly)  int64_t            tag;
@property(nonatomic, assign)            int                timeout;
@property(nonatomic, strong)            id                 userData;

-(instancetype)init;
-(instancetype)initWithTag:(int64_t)tag;

-(NSData*)addTag:(int64_t)tag toBinary:(NSMutableData*)data;
-(NSData*)createPackage;
@end
