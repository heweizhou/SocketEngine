//
//  NFCPResponseMessage.m
//  NFCPSyncEngine
//
//  Created by 周贺伟 on 2017/9/29.
//  Copyright © 2017年 周贺伟. All rights reserved.
//

#import "NFCPResponseMessage.h"

@implementation NFCPResponseMessage

- (int64_t)getMeesageTag
{
    return 0;
}

- (int64_t)getErrorCode
{
    return 0;
}

- (NSString*)getErrorMessage
{
    return @"";
}

- (NSString*)getUserContent
{
    return @"";
}

@end
