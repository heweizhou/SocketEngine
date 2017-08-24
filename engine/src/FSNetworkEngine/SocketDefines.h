//
//  SocketDefines.h
//  NetworkEngine
//
//  Created by 周贺伟 on 2017/7/10.
//  Copyright © 2017年 zhouhewei. All rights reserved.
//

#ifndef SocketDefines_h
#define SocketDefines_h


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
#define INNER_EVENT_CONNECTED_CREATE_SUCCESSED  (INNER_EVENT_CODE_BASE + 13)
#define INNER_EVENT_CONNECTED_SUCCESSED         (INNER_EVENT_CODE_BASE + 5)
#define INNER_EVENT_CONNECTED_FAILD             (INNER_EVENT_CODE_BASE + 6)
#define INNER_EVENT_CONNECTED_TIMEOUT           (INNER_EVENT_CODE_BASE + 7)
#define INNER_EVENT_CONNECTED_REFUSED           (INNER_EVENT_CODE_BASE + 8)
#define INNER_EVENT_CONNECTED_VAVAIL            (INNER_EVENT_CODE_BASE + 9)
#define INNER_EVENT_CONNECTED_USER_BREAK        (INNER_EVENT_CODE_BASE + 12)
/*********************/

/***********Socket 创建状态码**********/
#define INNER_EVENT_SOCKET_CREATE_FAILED     -100001
/*********************/

#endif /* SocketDefines_h */
