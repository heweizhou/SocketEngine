//
//  CBundleManager.h
//  commons
//
//  Created by 周贺伟 on 14/11/27.
//  Copyright (c) 2014年 周贺伟. All rights reserved.
//

#ifndef __commons__CBundleManager__
#define __commons__CBundleManager__

#include <stdio.h>
#include "CBundle.h"

#define  BundleManager  (CBundleManager::getInstance())
typedef  MAP<_HANDLE, CBundle*> BundleManagerMap;

class CBundleManager
{
public:
    static CBundleManager*      getInstance();
    static _VOID                destroy();
public:
    _HANDLE                     createBundle();
    _VOID                       destroyBundle( _HANDLE hander );
    CBundle*                    getObject( _HANDLE handler );
private:
    CBundleManager();
    ~CBundleManager();
private:
    static CBundleManager*    m_sInstance;
    static ICMutex*           m_sInstanceMutex;
private:
    ICMutex*                    m_mutex;
    BundleManagerMap            m_mCBundleManagerMap;
};

#endif /* defined(__commons__CBundleManager__) */
