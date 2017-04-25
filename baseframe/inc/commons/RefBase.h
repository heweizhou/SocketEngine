//
//  RefBase.h
//  commons
//
//  Created by 周贺伟 on 14/12/3.
//  Copyright (c) 2014年 周贺伟. All rights reserved.
//

#ifndef commons_RefBase_h
#define commons_RefBase_h
#include "defines.h"
#include "ICMutex.h"

namespace _MY_NAME_SPACE_ {
    class RefBase
    {
    public:
        RefBase();
        RefBase( _CONST RefBase& ref );
    protected:
        virtual ~RefBase();
    public:
        _BOOL   incRef();
        _VOID   release();
        _UINT   getRefCount();
    private:
        ICMutex*    m__mutex;
        _UINT       m__count;
        _BOOL       m__isReleaseing;
    };
}
#endif
