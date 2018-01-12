//
//  MessageDefines.h
//  MessageCenter
//
//  Created by 周贺伟 on 14/11/23.
//  Copyright (c) 2014年 周贺伟. All rights reserved.
//

#ifndef MessageCenter_MessageDefines_h
#define MessageCenter_MessageDefines_h

//#include "../common/defines.h"
#include "defines.h"

typedef unsigned int MessageType;

#define MESSAGE_BASE                    0x0
#define MESSAGE_COMMON                  0x1000
#define MESSAGE_ENGINE                  0x2000
#define MESSAGE_USER_0                  0x3000
#define MESSAGE_USER_1                  0x4000
#define MESSAGE_USER_2                  0x5000
#define MESSAGE_USER_3                  0x6000
#define MESSAGE_USER_4                  0x7000
#define MESSAGE_USER                    0x8000

#define MESSAGE_NONE                    (MESSAGE_BASE + 0)
#define MESSAGE_COM_EXIT                (MESSAGE_BASE + 1)

#endif
