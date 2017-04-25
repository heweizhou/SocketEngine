//
//  DataRequestProtocol.cpp
//  commons
//
//  Created by 周贺伟 on 15/11/18.
//  Copyright © 2015年 周贺伟. All rights reserved.
//

#include <stdio.h>
#include "DataRequestProtocol.h"

_HANDLE createDataBackBundleByShmHandler(_DataHubMessageKind kind, _HANDLE shmHandler, _PVOID reserve, _PVOID param, _ULONG param_len)
{
    _HANDLE     bundleHandler   = BundleManager->createBundle();
    CBundle*    bundle          = BundleManager->getObject( bundleHandler );
    
    bundle->setData( "MsgKind", &kind, sizeof(_DataHubMessageKind));
    bundle->setData( "memHandler", &shmHandler, sizeof(_HANDLE));
    bundle->setData( "reserve", &reserve, sizeof(_PVOID));
    bundle->setData( "param", param, param_len);
    
    return bundleHandler;
}

_HANDLE createDataBackBundleByBuffer(_DataHubMessageKind kind, _PVOID buffer, _ULONG bufferSize, _PVOID reserve, _PVOID param, _ULONG param_len)
{
    
    _HANDLE shmHandler = ShareMemManager->createSharedMemory(bufferSize);
    ICSharedMemory*  shm = ShareMemManager->getObject(shmHandler);
    
    if (shm) {
        _PVOID ptr = shm->obtainMemoryLock();
        SAFE_COPY(ptr, bufferSize, buffer, bufferSize);
        shm->releaseMemoryLock();
        
        return createDataBackBundleByShmHandler(kind, shmHandler, reserve, param, param_len);
    }
    return 0;
}

_HANDLE createEventInfoBundle(_DataHubMessageKind kind, _UINT code, _PVOID reserve, _PVOID param, _ULONG param_len)
{

    _HANDLE     bundleHandler   = BundleManager->createBundle();
    CBundle*    bundle          = BundleManager->getObject(bundleHandler);
    
    bundle->setData( "MsgKind", &kind, sizeof(_DataHubMessageKind));
    bundle->setData( "reserve", &reserve, sizeof(_PVOID));
    bundle->setData( "param", param, param_len);
    bundle->setData( "event_code", &code, sizeof(code));
    
    return bundleHandler;
}
