//
//  log.cpp
//  commons
//
//  Created by 周贺伟 on 15/7/22.
//  Copyright (c) 2015年 周贺伟. All rights reserved.
//

#include <stdio.h>
#include <stdarg.h>
#include "log.h"
#include "native_log.h"

#define  MAX_LOG_BUFFER_SIZE 512
namespace _MY_NAME_SPACE_
{
    extern "C"
    {
        //private
        _VOID vlog(_CONST _CHAR* info, _CONST _CHAR* tag, _CONST _CHAR* format, va_list arglist )
        {
            _CHAR msgBuffer[MAX_LOG_BUFFER_SIZE] = {0};
            _CHAR formatBuffer[MAX_LOG_BUFFER_SIZE] = {0};
            
            snprintf(formatBuffer, MAX_LOG_BUFFER_SIZE, "%s   %s:%s", info, tag, format );
            vsnprintf(msgBuffer, MAX_LOG_BUFFER_SIZE, formatBuffer, arglist);
            
            native_log( msgBuffer );
        }
        
        //public
        
        _VOID logi(_CONST _CHAR* tag, _CONST _CHAR* format, ... )
        {
#ifdef  DEBUG
            va_list arglist;
            va_start(arglist, format);
            vlog("info", tag, format, arglist);
            va_end(arglist);
#endif
        }
        _VOID loge(_CONST _CHAR* tag, _CONST _CHAR* format, ... )
        {
#ifdef  DEBUG
            va_list arglist;
            va_start(arglist, format);
            vlog("error", tag, format, arglist);
            va_end(arglist);
#endif
        }
        _VOID logv( _CONST _CHAR* tag, _CONST _CHAR* format, ... )
        {
#ifdef DEBUG
            va_list arglist;
            va_start(arglist, format);
            vlog("verbose", tag, format, arglist);
            va_end(arglist);
#endif
        }
        
        
    }
}