//
//  NFCPQueueUtils.m
//  NFCPSyncEngine
//
//  Created by 周贺伟 on 16/8/17.
//  Copyright © 2016年 fxiaoke. All rights reserved.
//

#import "NFCPQueueUtils.h"

@interface NFCPQueueUtils()

@property (nonatomic, strong) NSMutableDictionary* dic;
@end

@implementation NFCPQueueUtils

+ (NFCPQueueUtils *)sharedInstance
{
    static NFCPQueueUtils *sharedInstance = nil;
    static dispatch_once_t predicate;
    dispatch_once(&predicate, ^{
        sharedInstance = [[self alloc] init];
    });
    return sharedInstance;
}

-(instancetype)init
{
    self = [super init];
    if (self) {
        _dic = [[NSMutableDictionary alloc] init];
    }
    
    return self;
}

-(dispatch_queue_t)getGlobalQueue:(NSString*)name;
{
    @synchronized (_dic) {
        if (name && [name length] > 0) {
            dispatch_queue_t queue = [_dic valueForKey:name];
            if (!queue) {
                queue = dispatch_queue_create([name UTF8String], DISPATCH_QUEUE_SERIAL);
                [_dic setObject:queue forKey:name];
            }
            return queue;
        }
        return nil;
    }
}

@end
