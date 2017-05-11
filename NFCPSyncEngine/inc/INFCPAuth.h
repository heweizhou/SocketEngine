//
//  NFCPAuth.h
//  NFCPSyncEngine
//
//  Created by 周贺伟 on 16/6/3.
//  Copyright © 2016年 周贺伟. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Channel.h"
#import "IAuthDelegate.h"

@protocol INFCPAuth <NSObject>

-(void)startAuthWithChannel:(Channel*)channel;
-(void)setAuthDelegate:(id<IAuthDelegate>)authDelegate;

@end
