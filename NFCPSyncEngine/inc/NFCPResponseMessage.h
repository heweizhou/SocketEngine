//
//  NFCPResponseMessage.h
//  NFCPSyncEngine
//
//  Created by 周贺伟 on 2017/9/29.
//  Copyright © 2017年 周贺伟. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface NFCPResponseMessage : NSObject

- (int64_t)getMeesageTag;
- (int64_t)getErrorCode;
- (NSString*)getErrorMessage;
- (NSString*)getUserContent;

@end
