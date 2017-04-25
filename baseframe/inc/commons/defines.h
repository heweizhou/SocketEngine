//
//  defines.h
//  OpenglES1.1_engine
//
//  Created by 周贺伟 on 14/11/5.
//  Copyright (c) 2014年 zhouhewei. All rights reserved.
//

#ifndef OpenglES1_1_engine_defines_h
#define OpenglES1_1_engine_defines_h

#include <string>
#include <assert.h>
#include <limits.h>

#define _SERVICE_ENGINE_ASSIST      "RenderAssist"
#define _SERVICE_ENGINE             "RenderEngine"
#define _SERVICE_TEXURE_MANAGER     "TexureManager"
#define _SERVICE_DATAHUB            "DataHub"

#define _PI_            M_PI
#define _NULL           0
#define _CONST          const
#define _TRUE           true
#define _FALSE          false

#define  _MY_NAME_SPACE_   fx_common

#ifdef __GXX_EXPERIMENTAL_CXX0X__
//#if(__cplusplus == 201103L)
//#include <unordered_map>
//#include <unordered_set>
//#define  MAP unordered_map
//#define  SET unordered_set
//#else
#include <tr1/unordered_map>
#include <tr1/unordered_set>
#define  MAP std::tr1::unordered_map
#define  SET std::tr1::unordered_set
//#endif
#else
#include <map>
#include <set>
#define  MAP std::map
#define  SET std::set
#endif

#ifndef ANDROID_SYSTEM
//#define USE_LIB_HTTP
//#define USE_LIB_JPEG
#endif

#define SAFE_COPY(destMem, destMemSize, srcMem, srcMemSize ) \
do{\
    if ( destMemSize >= srcMemSize )\
    {\
        memcpy( destMem, srcMem, srcMemSize );\
    }\
    else\
    {\
        _PVOID __ptr = destMem;\
        __ptr = realloc( __ptr, srcMemSize );\
        \
        if(__ptr == _NULL )\
        {\
            perror("memory error !");\
            assert(__ptr);\
        }\
        \
        destMem = __ptr;\
        memcpy( destMem, srcMem, srcMemSize );\
    }\
}while(0);

#define SAFE_FREE(__ptr)       \
do\
{\
    if( __ptr != _NULL )\
    {\
        free(__ptr);\
        __ptr = _NULL;\
    }\
}while(0);

#define SAFE_DELETE(__ptr)       \
do\
{\
if( __ptr != _NULL )\
{\
    delete __ptr;\
    __ptr = _NULL;\
}\
}while(0);

#define SAFE_DELETE_ARR(__ptr)       \
do\
{\
if( __ptr != _NULL )\
{\
delete[] __ptr;\
__ptr = _NULL;\
}\
}while(0);

#define SAFE_RELEASE(__ptr)       \
do\
{\
if( __ptr != _NULL )\
{\
__ptr->release();\
__ptr = _NULL;\
}\
}while(0);

enum thread_priority
{
    LOW,
    NORMAL,
    HIGH
};

typedef void            _VOID;
typedef bool            _BOOL;
typedef char            _CHAR;
typedef signed char     _SCHAR;
typedef unsigned char   _UCHAR;
typedef short           _SHORT;
typedef unsigned short  _USHORT;
typedef signed int      _INT;
typedef unsigned int    _UINT;
typedef float           _FLOAT;
typedef double          _DOUBLE;
typedef long            _LONG;
typedef unsigned long   _ULONG;

typedef void*           _PVOID;
//typedef _CHAR*          _PPCHAR;
typedef signed char*    _PSCHAR;
typedef unsigned char*  _PUCHAR;
typedef short*          _PSHORT;
typedef unsigned short* _PUSHORT;
typedef signed int*     _PINT;
typedef unsigned int*   _PUINT;
typedef float*          _PFLOAT;
typedef double*         _PDOUBLE;
typedef long*           _PLONG;
typedef unsigned long*  _PULONG;

typedef _LONG           _HANDLE;
typedef _INT            _PID;
typedef _ULONG          _TID;
typedef _UINT           _DataHubMessageKind;
typedef _ULONG          _TIMESTAMP;

typedef int             _SOCKET_FD;
typedef int             _SOCKET_PORT;
typedef int             _MUX_COM_HANDLER;

#define _HANDLE_MAX     (LONG_MAX)
#define _string         ::std::string

#define TOINT(value)               static_cast<_INT>(value)
#define TOUINT(value)              static_cast<_UINT>(value)
#define TOCHAR(value)              static_cast<_CHAR>(value)
#define TOFLOAT(value)             static_cast<_FLOAT>(value)
#define TOLONG(value)              static_cast<LONG>(value)
#define TODOUBLE(value)            static_cast<DOUBLE>(value)

#define CAST(kind,value)           static_cast<kind>(value)

#define  DATA_HUB_MESSAGEKIND_IMAGE     1
#define  DATA_HUB_MESSAGEKIND_USER      0x1000

#endif
