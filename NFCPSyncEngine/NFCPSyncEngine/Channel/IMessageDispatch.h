//
//  IMessageDispatch.h
//  FSNetWorkEngine
//
//  Created by Lin on 16/6/4.
//  Copyright © 2016年 fxiaoke. All rights reserved.
//

#import <Foundation/Foundation.h>

@protocol IMessageDispatch <NSObject>

-(void)dispatchMessage:(NSData*)data;

@end
