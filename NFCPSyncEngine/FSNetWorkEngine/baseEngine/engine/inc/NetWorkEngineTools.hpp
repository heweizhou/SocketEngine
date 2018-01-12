//
//  NetWorkEngineTools.hpp
//  NetworkEngine
//
//  Created by 周贺伟 on 2017/5/19.
//  Copyright © 2017年 zhw. All rights reserved.
//

#ifndef NetWorkEngineTools_hpp
#define NetWorkEngineTools_hpp

#include <stdio.h>

class CNetWorkEngineTools {
public:
    static long CreateShareMem(const void* buffer, const long buffer_size);
    static void ReleaseShareMem(const long handler);
};

#endif /* NetWorkEngineTools_hpp */
