//
//  FsDDLog.h
//  FaciShare
//
//  Created by ly on 15/9/17.
//  Copyright (c) 2015年 周贺伟. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface FsDDLog : NSObject

+ (void)FsLogInfo:(NSString *)log;
+ (void)FsLogError:(NSString *)log;

@end
