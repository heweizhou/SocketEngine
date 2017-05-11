//
//  NFCPDefines.h
//  NFCPSyncEngine
//
//  Created by 周贺伟 on 2017/3/7.
//  Copyright © 2017年 fxiaoke. All rights reserved.
//

#ifndef NFCPDefines_h
#define NFCPDefines_h

typedef enum {
    FSSocketBadConfigError          = -100001,         //Invalid configuration（创建socket失败）
    FSSocketConnectRefuesd          = -100002,         //socket连接被拒绝
    FSSocketConnectAddressTavail    = -100003,         //远程地址无效。通常处理方法是选择新的地址，并重新连接或提示用户，关闭套接字并终止程序。
    FSSocketOtherError              = -100004          //其他错误
} FSSocketError;

typedef unsigned char   _BYTE;
typedef short int       _WORD;


#endif /* NFCPDefines_h */
