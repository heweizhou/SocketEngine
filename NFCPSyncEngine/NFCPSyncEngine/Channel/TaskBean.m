//
//  TaskBean.m
//  FSNetWorkEngine
//
//  Created by 周贺伟 on 16/5/18.
//  Copyright © 2016年 fxiaoke. All rights reserved.
//

#import "TaskBean.h"
#import "TaskBean+Private.h"
#import "IDataRequest.h"

@interface MsgID : NSObject
+ (int64_t)getMsgID;
@end


@implementation MsgID

+ (int64_t)getMsgID
{
    static int64_t msgId = 0;
    @synchronized(self) {
        return ++msgId;
    }
}

@end

@interface TaskBean()<IDataRequest>
{
    int64_t         _tag;
    id              _channel;
}
@end

@implementation TaskBean

-(instancetype)init
{
    self = [self initWithTag:[MsgID getMsgID]];
    if (self) {
        [self initialisation];
    }
    
    return self;
}

-(instancetype)initWithTag:(int64_t)tag
{
    self = [super init];
    if (self) {
        _tag = tag;
    }
    
    return self;
}

#pragma --mark private function
-(void)initialisation
{
    _data = [[NSMutableData alloc] init];
    _channel = nil;
    _timeout = 60;
}

-(int64_t)getTag
{
    return _tag;
}

-(id)getChannel
{
    return _channel;
}

-(void)setChannel:(id)Channel
{
    _channel = Channel;
}

-(NSData*)generateBinaryData
{
    return [self createPackage];
}

-(NSData*)createPackage
{
    //do nothing
    return nil;
}

- (NSString *)description
{
    return [NSString stringWithFormat:@"<%@: %p> tag=%lld", [self class], self, _tag];
}

- (NSString *)debugDescription
{
    return [NSString stringWithFormat:@"<%@: %p> tag=%lld", [self class], self, _tag];
}

@end
