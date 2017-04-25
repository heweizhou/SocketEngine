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

/***********Socket 请求状态码**********/

#define INNER_EVENT_CODE_BASE   0x100
#define USER_EVENT_CODE_BASE    0x200

#define INNER_EVENT_DEFAULT             (INNER_EVENT_CODE_BASE + 1)
#define INNER_EVENT_SEND_SUCCESSED      (INNER_EVENT_CODE_BASE + 2)
#define INNER_EVENT_SEND_FAILD          (INNER_EVENT_CODE_BASE + 3)
#define INNER_EVENT_SEND_PACKAGE_INVALID (INNER_EVENT_CODE_BASE + 11)
/*********************/

/***********Socket 错误码**********/
#define INNER_EVENT_SOCKET_CLOSED       (INNER_EVENT_CODE_BASE + 4)
#define INNER_EVENT_SOCKET_SELECT_ERROR (INNER_EVENT_CODE_BASE + 10)
/*********************/

/***********Socket 连接状态码**********/
#define INNER_EVENT_CONNECTED_SUCCESSED      (INNER_EVENT_CODE_BASE + 5)
#define INNER_EVENT_CONNECTED_FAILD          (INNER_EVENT_CODE_BASE + 6)
#define INNER_EVENT_CONNECTED_TIMEOUT        (INNER_EVENT_CODE_BASE + 7)
#define INNER_EVENT_CONNECTED_REFUSED        (INNER_EVENT_CODE_BASE + 8)
#define INNER_EVENT_CONNECTED_VAVAIL         (INNER_EVENT_CODE_BASE + 9)
#define INNER_EVENT_CONNECTED_USER_BREAK     (INNER_EVENT_CODE_BASE + 12)
/*********************/

/***********Socket 创建状态码**********/
#define INNER_EVENT_SOCKET_CREATE_FAILED     -100001
/*********************/

class IHubRequestBack;

extern "C"
{
    _HANDLE createDataBackBundleByShmHandler(_DataHubMessageKind kind, _HANDLE shmHandler, _PVOID reserve, _PVOID param, _ULONG param_len);
    _HANDLE createDataBackBundleByBuffer(_DataHubMessageKind kind, _PVOID buffer, _ULONG bufferSize, _PVOID reserve, _PVOID param, _ULONG param_len);
    _HANDLE createEventInfoBundle(_DataHubMessageKind kind, _UINT code, _PVOID reserve, _PVOID param, _ULONG param_len);
 }

#endif
