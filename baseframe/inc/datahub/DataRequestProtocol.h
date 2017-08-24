//
//  DataRequestProtocol.h
//  commons
//
//  Created by 周贺伟 on 15/11/18.
//  Copyright (c) 2015年 周贺伟. All rights reserved.
//

#ifndef commons_DataRequestProtocol_h
#define commons_DataRequestProtocol_h


#include "RefBase.h"
#include "CBundleManager.h"
#include "CShareMemManager.h"
#include "MessageDefines.h"

/**********DataHub 功能码***********/
#define   MESSAGE_DATAHUB_LOAD_COMPLETE         (MESSAGE_COMMON + 1)
//#define   MESSAGE_DATAHUB_NEW_HTTP_REQUEST      (MESSAGE_COMMON + 2)
//
//#define   MESSAGE_DATAHUB_SOCKET_CONNECT        (MESSAGE_COMMON + 3)
//#define   MESSAGE_DATAHUB_NEW_SOCKET_REQUEST    (MESSAGE_COMMON + 4)

#define   MESSAGE_DATAHUB_CLEAR_TASKS           (MESSAGE_COMMON + 5)
#define   MESSAGE_DATAHUB_ON_EVENT              (MESSAGE_COMMON + 6)


#define   MESSAGE_DATAHUB_REG_MSG_KIND          (MESSAGE_COMMON + 7)
#define   MESSAGE_DATAHUB_UNREG_MSG_KIND        (MESSAGE_COMMON + 8)
#define   MESSAGE_DATAHUB_CLEAR                 (MESSAGE_COMMON + 9)
/**********DataHub 功能码***********/

class IHubRequestBack;

extern "C"
{
    _HANDLE createDataBackBundleByShmHandler(_DataHubMessageKind kind, _HANDLE shmHandler, _PVOID reserve, _PVOID param, _ULONG param_len);
    _HANDLE createDataBackBundleByBuffer(_DataHubMessageKind kind, _PVOID buffer, _ULONG bufferSize, _PVOID reserve, _PVOID param, _ULONG param_len);
    _HANDLE createEventInfoBundle(_DataHubMessageKind kind, _UINT code, _PVOID reserve, _PVOID param, _ULONG param_len);
 }

#endif
