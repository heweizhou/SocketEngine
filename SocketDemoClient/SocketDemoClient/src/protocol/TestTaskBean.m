//
//  TestTaskBean.m
//  SocketDemoClient
//
//  Created by 周贺伟 on 2018/1/12.
//  Copyright © 2018年 zhouhewei. All rights reserved.
//

#import "TestTaskBean.h"
#import "Protocol.h"

@interface TestTaskBean ()
{
    NSString*       _data;
}
@end

@implementation TestTaskBean

- (instancetype)initWithData:(NSString*)msg
{
    self = [super init];
    if (self) {
        _data = msg;
    }
    return self;
}

-(NSData*)addTag:(int64_t)tag toBinary:(NSMutableData*)data;
{
    package_header header;
    header.tag = tag;
    header.package_size = data.length;
    
    NSMutableData* package_data = [[NSMutableData alloc] initWithBytes:&header length:sizeof(header)];
    [package_data appendData:data];
    return package_data;
}

-(NSData*)createPackage
{
    return [_data dataUsingEncoding:NSUTF8StringEncoding];
}

@end
