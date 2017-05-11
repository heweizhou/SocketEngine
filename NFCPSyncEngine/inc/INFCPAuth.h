//
//  NFCPAuth.h
//  NFCPSyncEngine
//
//  Created by Lin on 16/6/3.
//  Copyright © 2016年 fxiaoke. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Channel.h"
#import "IAuthDelegate.h"

@protocol INFCPAuth <NSObject>

-(void)startAuthWithChannel:(Channel*)channel;
-(void)setAuthDelegate:(id<IAuthDelegate>)authDelegate;

@end
