//
//  Protocol.h
//  SocketDemoClient
//
//  Created by 周贺伟 on 2018/1/12.
//  Copyright © 2018年 zhouhewei. All rights reserved.
//

#ifndef Protocol_h
#define Protocol_h

typedef struct _tag_package_header
{
    int64_t tag;
    long    package_size;
}package_header;



/*
 
 #!@# + package header data + body data + #@!#
 
*/

#endif /* Protocol_h */
