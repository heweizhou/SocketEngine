//
//  SocketDefines.h
//  FCPNetWork
//
//  Created by 周贺伟 on 15/12/22.
//  Copyright © 2015年 周贺伟. All rights reserved.
//

#ifndef SocketDefines_h
#define SocketDefines_h

#define getKey(kind) [NSString stringWithFormat:@"fx_msg_%ld", kind]


typedef struct tag_n_package_param
{
    long     msg_kind;
    long     package_param;
}n_package_param;

typedef enum  _tag_socket_msg_kind{
    _reserved = 0,              /**< 保留*/
    _socketDidClosed,           /**< 连接关闭*/
    _onReceiveData,             /**< 接收到数据*/
    _socketSendSuccessed,       /**< 发送数据成功*/
    _socketSendSuccessedFailed, /**< 发送数据失败*/
    _socketSendPackageInvalid,  /**< 数据包无效*/
    _socketConnectSuccessed,    /**< 连接成功*/
    _socketConnectFailed,       /**< 连接失败*/
    _socketConnectTimeout,      /**< 连接超时*/
    _socketConnectUserBreak,    /**< 用户取消*/
    _sockeErrorCallBack         /**< 其他错误*/
}socket_msg_kind;

#define N_SOCKET_FD @"socket_fd"
#define N_SOCKET_MSG_KIND @"socket_msg_kind"
#define N_SOCKET_HANDLER @"socket_hander"
#define N_SOCKET_DATA_BUFFER @"data"
#define N_SOCKET_ERROR @"socketError"
#define N_SOCKET_PARAM @"param"

#define N_POST_PARAM(fd, kind, data, error, param) \
@{N_SOCKET_FD:@(fd),\
N_SOCKET_MSG_KIND:@(kind),\
N_SOCKET_DATA_BUFFER:data,\
N_SOCKET_ERROR:@(error),\
N_SOCKET_PARAM:param}

#endif /* defines_h */
