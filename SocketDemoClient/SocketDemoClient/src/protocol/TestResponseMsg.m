//
//  TestResponseMsg.m
//  SocketDemoClient
//
//  Created by 周贺伟 on 2018/1/12.
//  Copyright © 2018年 zhouhewei. All rights reserved.
//

#import "TestResponseMsg.h"
#import "Protocol.h"

@interface TestResponseMsg ()
{
    int64_t         _tag;
    NSString*       _content;
}
@end

@implementation TestResponseMsg

- (instancetype)initWithRawData:(NSData*)data
{
    self = [super init];
    if (self) {
        [self parseWithRawData:data];
    }
    return self;
}

#pragma --mark parser
- (void)parseWithRawData:(NSData*)data
{
    const void* rawBuffer = [data bytes];
    
    package_header* header = (package_header*)rawBuffer;
    _tag = header->tag;
    
    _content = [[NSString alloc] initWithBytes:rawBuffer + sizeof(package_header) length:header->package_size encoding:NSUTF8StringEncoding];
}

- (int64_t)getMeesageTag
{
    return _tag;
}

- (NSString*)getPackageContent
{
    return _content;
}


//- (int64_t)getErrorCode
//{
//
//}
//
//- (NSString*)getErrorMessage
//{
//
//}
//
//- (NSString*)getUserContent
//{
//
//}

@end
