//
//  CHanderManager.h
//  commons
//
//  Created by 周贺伟 on 14/11/26.
//  Copyright (c) 2014年 周贺伟. All rights reserved.
//

#ifndef __commons__CHanderManager__
#define __commons__CHanderManager__

#include <stdio.h>
#include <queue>
#include "defines.h"
#include "ICMutex.h"

#define HandlerManager (CHanderManager::getInstance())
typedef std::queue< _HANDLE > handerQueue;

class CHanderManager
{
public:
    static CHanderManager*  getInstance();
    static _VOID            destroy();
public:
    _HANDLE                 getFreeHander();
    _VOID                   destroyHander( _HANDLE hander );
private:
    CHanderManager();
    ~CHanderManager();
private:
    static CHanderManager*  m_sInstance;
    static ICMutex*         m_sInstanceMutex;
private:
    _UINT                   m_handersCount;
    ICMutex*                m_mutex;
    handerQueue             m_qUnusedHanders;
};
#endif /* defined(__commons__CHanderManager__) */
