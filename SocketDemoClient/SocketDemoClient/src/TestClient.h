//
//  TestClient.h
//  SocketDemoClient
//
//  Created by 周贺伟 on 2018/1/12.
//  Copyright © 2018年 zhouhewei. All rights reserved.
//

#import <Foundation/Foundation.h>

@protocol serverResponseDelegate <NSObject>

- (void)onMessageArrival:(NSString*)msg;

@end

@interface TestClient : NSObject

@property (nonatomic, weak) id<serverResponseDelegate> delegate;


- (void)sendMessage:(NSString*)msg;

@end
