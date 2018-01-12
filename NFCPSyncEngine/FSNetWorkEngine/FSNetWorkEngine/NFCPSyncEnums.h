//
//  NFCPSyncEnums.h
//  NFCPSyncEngine
//
//  Created by ly on 16/1/27.
//  Copyright © 2016年 lyuan. All rights reserved.
//

#ifndef NFCPSyncEnums_h
#define NFCPSyncEnums_h

typedef enum {
    NReserved = 0,              /**< 保留*/
    NSocketSendSuccessed,       /**< 发送数据成功*/
    NSocketSendSuccessedFailed, /**< 发送数据失败*/
    NSocketSendPackageInvalid   /**< 数据包无效*/
}NSocketSendStatus;

typedef enum{
    NFSSocketBadConfigError          = -100001,         //Invalid configuration（创建socket失败）
    NFSSocketConnectRefuesd          = -100002,         //socket连接被拒绝
    NFSSocketConnectAddressTavail    = -100003,         //远程地址无效。通常处理方法是选择新的地址，并重新连接或提示用户，关闭套接字并终止程序。
    NFSSocketOtherError              = -100004          //其他错误
}NFSSocketError;

#endif /* NFCPSyncEnums_h */
