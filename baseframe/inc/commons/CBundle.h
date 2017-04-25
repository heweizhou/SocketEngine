//
//  CBundle.h
//  commons
//
//  Created by 周贺伟 on 14/11/27.
//  Copyright (c) 2014年 周贺伟. All rights reserved.
//

#ifndef __commons__CBundle__
#define __commons__CBundle__

#include "defines.h"
#include "CMutexFactory.h"
#include <assert.h>

typedef  struct _STRUCT_BUNDLE_VALUE
{
    _STRUCT_BUNDLE_VALUE( _PVOID buf, _ULONG size )
    {
        if (size == 0){
            buffer = _NULL;
            bufferSize = 0;
        }else{
            buffer = malloc( size );
            assert( buffer );
            bufferSize = size;
            memcpy( buffer, buf, size );
        }
        
    }
    
    ~_STRUCT_BUNDLE_VALUE()
    {
        SAFE_FREE( buffer );
    }
    
    _VOID setData( _PVOID buf, _ULONG size )
    {
        if (size == 0){
            SAFE_FREE(buffer);
            bufferSize = 0;
        }else{
            if(  bufferSize < size ){
                _PVOID tmp = realloc( buffer, size );
                assert( tmp );
                buffer = tmp;
            }
            bufferSize = size;
            memcpy( buffer, buf, size);
        }
    }
    
    _ULONG      bufferSize;
    _PVOID      buffer;
}BundleValue;

typedef MAP< _string, BundleValue* > BundleMap;

class CBundle
{
public:
    _VOID   setData( _string key, _PVOID buffer, _ULONG size );
    _PVOID  getData( _string key, _ULONG& size );
    _VOID   lock();
    _VOID   unlock();
public:
    CBundle();
    ~CBundle();
private:
    ICMutex*        m_mutex;
    BundleMap       m_mBundle;
};
#endif /* defined(__commons__CBundle__) */
