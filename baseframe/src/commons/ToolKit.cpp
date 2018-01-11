//
//  ToolKit.cpp
//  commons_mac
//
//  Created by 周贺伟 on 2016/11/18.
//  Copyright © 2016年 周贺伟. All rights reserved.
//

#include "ToolKit.hpp"

_string BKDRHash(const _string& key)
{
    unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
    unsigned int hash = 0;
    
    char* str = (char*)key.c_str();
    
    while (*str)
    {
        hash = hash * seed + (*str++);
    }
    
    hash = hash & 0x7FFFFFFF;
    
    char str_hash_code[64] = {0};
    sprintf(str_hash_code, "%d", hash);
    
    return str_hash_code;
}
