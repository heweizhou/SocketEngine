//
//  NFCPQueueUtils.h
//  NFCPSyncEngine
//
//  Created by 周贺伟 on 16/8/17.
//  Copyright © 2016年 周贺伟. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface NFCPQueueUtils : NSObject

+(NFCPQueueUtils *)sharedInstance;

-(dispatch_queue_t)getGlobalQueue:(NSString*)name;

@end
