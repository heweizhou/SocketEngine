//
//  IMessageDispatch.h
//  FSNetWorkEngine
//
//  Created by 周贺伟 on 16/6/4.
//  Copyright © 2016年 周贺伟. All rights reserved.
//

#import <Foundation/Foundation.h>

@protocol IMessageDispatch <NSObject>

-(void)dispatchMessage:(NSData*)data;

@end
