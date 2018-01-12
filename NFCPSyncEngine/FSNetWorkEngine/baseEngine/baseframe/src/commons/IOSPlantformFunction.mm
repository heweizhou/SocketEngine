////
////  CNetFactory.cpp
////  commons
////
////  Created by 周贺伟 on 14/12/3.
////  Copyright (c) 2014年 周贺伟. All rights reserved.
////
//
//#include <stdio.h>
////#include "CNetFactory.h"
////#include "CFileFactory.h"
//#include "CImageFunctions.h"
//
//#include "native_log.h"
//#include "CShareMemManager.h"
//#include <stdarg.h>
//#import <Foundation/Foundation.h>
//#import <Foundation/Foundation.h>
////#import <UIKit/UIKit.h>
//
//#ifdef USE_LIB_HTTP
//#include "ghttplib/ghttp.h"
//#endif
//
//#ifdef USE_LIB_HTTP
//
//_HANDLE CNetFactory::HTTP_GET( _string url )
//{
//    int tryTime = 3;
//    ghttp_request *request = NULL;
//    ghttp_status status;
//    
//    _HANDLE handler = 0;
//    while( tryTime-- )
//    {
//        request = ghttp_request_new();
//        
//        if( ghttp_set_uri(request, (char *) url.c_str()) == -1)
//            continue;
//        /* Close the connection after you are done. */
//        if( ghttp_set_type(request, ghttp_type_get) == -1 )
//            continue;
//        
//        ghttp_set_header(request, http_hdr_Connection, "close");
//        ghttp_prepare(request);
//        status = ghttp_process(request);
//        
//        if( status == ghttp_error )
//            continue;
//        
//        if( ghttp_status_code(request) == 200 )
//        {
//            char* olddata = ghttp_get_body(request);
//            _UINT oldsize = ghttp_get_body_len(request);
//            handler = ShareMemManager->createSharedMemory(oldsize);
//            ICSharedMemory* sm = ShareMemManager->getObject(handler);
//            _PVOID buffer = sm->obtainMemoryLock();
//            memcpy(buffer, olddata, oldsize);
//            sm->releaseMemoryLock();
//            
//            ghttp_request_destroy(request);
//            break;
//        }
//        ghttp_request_destroy(request);
//    }
//    
//    return handler;
//}
//#else
//
////inline _HANDLE getShareMemHandler( NSData* nsData )
////{
////    if (nsData)
////    {
////        _HANDLE handler = ShareMemManager->createSharedMemory( [nsData length] );
////        ICSharedMemory* sm = ShareMemManager->getObject( handler );
////
////        if( sm != _NULL )
////        {
////            _PVOID buffer = sm->obtainMemoryLock();
////            memcpy( buffer, [nsData bytes], [nsData length] );
////            sm->releaseMemoryLock();
////            
////        }
////        return handler;
////    }
////    
////    return 0;
////}
//
//_HANDLE CNetFactory::HTTP_GET( _string url )
//{
////    _INT trytime                = 3;
////    while (trytime--)
////    {
////        Class BaiduInterface = NSClassFromString(INTERFACE_NAME_CLASS);
////        SEL func = NSSelectorFromString(INTERFACE_FUNC_GETDATA);
////        
////        if (BaiduInterface&&[BaiduInterface respondsToSelector:func])
////        {
////            //不影响内存
////            _HANDLE handler = 0;
////            @autoreleasepool {
////                NSData* nsData = [BaiduInterface performSelector:func withObject:[NSString stringWithUTF8String:url.c_str()]];
////                handler = getShareMemHandler(nsData);
////            }
////            return handler;
////        }
////        else
////        {
////            NSString* ns_url    = [[NSString alloc] initWithUTF8String:url.c_str()];
////            NSURL* tileURL      = [[NSURL alloc] initWithString:ns_url ];
////            NSData * nsData     = [[NSData alloc] initWithContentsOfURL:tileURL];
////        
////            [tileURL release];
////            [ns_url  release];
////            
////            _HANDLE handler  = getShareMemHandler( nsData );
////            [nsData release];
////            
////            return handler;
////        }
////
////        
////    }
//    return 0;
//}
//
//#endif
//
//extern "C"
//{
//    _VOID native_log( _CHAR* message )
//    {
//#ifdef  DEBUG
//        NSLog(@"%s", message );
//#endif
//    }
//}
//
//_HANDLE  CFileFactory::FILE_GET( _string url )
//{
//    return 0;
//}
//
//
////_string GetHashCode(const _string& key)
////{
////    int hashcode = 0;
////    int kelength = (int) key.length();
////    
////    int COUNT = (kelength >> 2) + ((kelength & 3) ? 1 : 0);
////    
////    int data[1024];
////    
////    assert(sizeof(int) * 4 > COUNT);
////    
////    memcpy(((char*) data) + 0, key.c_str(), kelength);
////    memset(((char*) data) + kelength, kelength, (COUNT << 2) - kelength);
////    
////    for (int n = 0; n < COUNT; n++)
////    {
////        hashcode += (data[n] << n);
////    }
////    
////    char str_hash_code[64] = {0};
////    sprintf(str_hash_code, "%d", hashcode);
////    
////    return str_hash_code;
////}
//
//
