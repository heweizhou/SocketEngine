//
//  SocketFunctions.hpp
//  SocketEngine
//
//  Created by 周贺伟 on 2017/3/10.
//  Copyright © 2017年 fxiaoke. All rights reserved.
//

#ifndef SocketFunctions_hpp
#define SocketFunctions_hpp

#include <stdio.h>
#include "defines.h"

typedef enum _SOCKET_CONNECT_STATUS{
    _CONNECT_STATUS_SUCEESS,
    _CONNECT_STATUS_FAILED  = 1,
    _CONNECT_STATUS_TIMEOUT = 2,
    _CONNECT_STATUS_BREAK = 3
}_socket_connect_status;


_UINT create_socket(_INT protocal, _INT domain);
/**
 @brief 设置ip地址和端口完成socket连接
 @param ipAddress    ip地址
 @param port         端口
 @return 返回创建socket FD
 */
_socket_connect_status connect_to_host(_UINT fd, _CONST _CHAR* ipAddress, _CONST _UINT port, _CONST _INT timeout_second);

/**
 @brief 返回当前的socket是否connect
 @param socketFD     已经创建的socket fd
 @return true-链接， false-断开
 */
_BOOL is_connected(_UINT fd);

/**
 *	@brief	 发送数据
 *	@param  binaryData 发送的数据
 *  @param  dataLength 发送数据包长度
 *  @return:
 1)write的返回值大于0,表示写了部分或者是全部的数据. 这样我们用一个while循环来不停的写入，但是循环过程中的buf参数和nbyte参数得由我们来更新。也就是说，网络写函数是不负责将全部数据写完之后在返回的。
 2)返回的值小于0,此时出现了错误.我们要根据错误类型来处理.
 如果错误为EINTR表示在写的时候出现了中断错误.
 如果为EPIPE表示网络连接出现了问题(对方已经关闭了连接).
 */
_LONG send_data(_UINT fd, _CONST _CHAR *binaryData,_LONG dataLength);


/**
 *	@brief	 接收数据
 *	@param  binaryData 发送的数据
 *  @param  dataLength 接收数据包长度
 *  @return read函数是负责从fd中读取内容.当读成功时,read返回实际所读的字节数,如果返回的值是0 表示已经读到文件的结束了,小于0表示出现了错误
 */
_LONG receive_data(_UINT fd, _CONST _CHAR *binaryData, _LONG &dataLength);


/**
 *	@brief	 断开socket连接
 *  @param socketFD    销毁创建的socket fd
 *  @return true-关闭成功， false-关闭失败
 */
_BOOL       disconnect_socket(_UINT fd);

#endif /* SocketFunctions_hpp */
