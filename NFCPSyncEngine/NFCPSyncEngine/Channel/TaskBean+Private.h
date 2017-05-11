//
//  TaskBean+Tag.h
//  NFCPSyncEngine
//
//  Created by 周贺伟 on 2017/3/30.
//  Copyright © 2017年 周贺伟. All rights reserved.
//

#ifndef TaskBean_Tag_h
#define TaskBean_Tag_h


@interface TaskBean(Private)
-(int64_t)getTag;

-(id)getChannel;
-(void)setChannel:(id)Channel;
@end

#endif /* TaskBean_Tag_h */
