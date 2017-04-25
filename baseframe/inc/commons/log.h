//
//  log.h
//  commons
//
//  Created by 周贺伟 on 15/7/22.
//  Copyright (c) 2015年 周贺伟. All rights reserved.
//

#ifndef commons_log_h
#define commons_log_h

#include "defines.h"

namespace _MY_NAME_SPACE_
{
    extern "C"
    {
        _VOID logi(_CONST _CHAR* tag, _CONST _CHAR* format, ... );
        _VOID loge(_CONST _CHAR* tag, _CONST _CHAR* format, ... );
        _VOID logv(_CONST _CHAR* tag, _CONST _CHAR* format, ... );
    }
}

#endif
