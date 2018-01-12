//
//  NetWorkEngineTools.cpp
//  NetworkEngine
//
//  Created by 周贺伟 on 2017/5/19.
//  Copyright © 2017年 zhw. All rights reserved.
//

#include "NetWorkEngineTools.hpp"
#include "CShareMemManager.h"

long CNetWorkEngineTools::CreateShareMem(const void* buffer, const long buffer_size)
{
    if (buffer_size <= 0) {
        return 0;
    }
    
    if (!buffer) {
        return 0;
    }
    
    _HANDLE handler = ShareMemManager->createSharedMemory(buffer_size);
    ICSharedMemory* shm = ShareMemManager->getObject(handler);
    _PVOID ptr = shm->obtainMemoryLock();
    SAFE_COPY(ptr, shm->getlength(), buffer, buffer_size);
    shm->releaseMemoryLock();
    
    return handler;
}


void CNetWorkEngineTools::ReleaseShareMem(const long handler)
{
    ShareMemManager->destroySharedMemory(handler);
}
