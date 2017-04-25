//
//  FCPNetWork.m
//  FCPNetWork
//
//  Created by 周贺伟 on 15/12/22.
//  Copyright © 2015年 周贺伟. All rights reserved.
//

#import "FSFCPNetWork.h"
#import "IFSNetworkEngine.h"
#import "SocketDefines.h"
#import "NObjcCallBack.h"
#include "netdb.h"
#import <UIKit/UIKit.h>

#define  IP_MAX_LENGTH  128

@interface FSFCPNetWork()
@property(nonatomic, strong) NSMutableDictionary* observers;
@property(nonatomic, assign) NObjcCallBack* CPPCallBack;
@end

@implementation FSFCPNetWork


+ (FSFCPNetWork *)sharedNetWorkManager
{
    static FSFCPNetWork *sharedInstance = nil;
    static dispatch_once_t predicate;
    dispatch_once(&predicate, ^{
        sharedInstance = [[self alloc] init];
    });
    return sharedInstance;
}

-(id)init
{
    self = [super init];
    if (self) {
        self.observers = [[NSMutableDictionary alloc] init];
        
    }
    
    return self;
}

-(void)dealloc
{
    delete self.CPPCallBack;
}

#pragma -- mark public function

-(void)registerSokcet:(id)obj ip:(NSString*)ip port:(unsigned int)port timeout:(int)timeout msgKind:(long)kind selector:(SEL)selector connectMethodIsAsync:(BOOL)isAsync
{
    NSString* key = getKey(kind);
    NSNotificationCenter *center = [NSNotificationCenter defaultCenter];
    [center addObserver:obj selector:selector name:key object:nil];
    
    [self addObject:key object:obj];
    [self connect:kind ip:ip port:port timeout:timeout connect:isAsync];
}

-(void)unRegisterSokcet:(id)obj msgKind:(long)kind;
{
    NSString* key = getKey(kind);
    NSNotificationCenter *center = [NSNotificationCenter defaultCenter];
    [center removeObserver:obj name:key object:nil];
    
    @synchronized (self.observers) {
        [self removeObject:key object:obj];
    }
}

-(BOOL)async_send:(long)msgKind buffer:(NSData*)data param:(id)param pri:(BOOL)isHighPri
{
    unsigned long handler = 0;
    @synchronized (self.observers) {
        NSString* key = getKey(msgKind);
        NSObject* _obj = [self.observers objectForKey:key];
        if (_obj) {
//            handler = ((FSFCPClientInfo*)_obj).SocketHandler;
        }
    }
    
    if (handler) {
        n_package_param param_package = {msgKind, (long)CFBridgingRetain(param)};
        return getEngineInstance()->async_sendData(handler, (const char*)[data bytes], [data length], &param_package, sizeof(n_package_param), isHighPri);
    }
    
    return NO;
}

-(id)connectionIsExsit:(long)msgKind
{
    @synchronized (self.observers) {
        NSString *key = getKey(msgKind);
//        FSFCPClientInfo *ci = self.observers[key];
//        return ci;
    }
    return nil;
}

- (BOOL)isConnected:(id)connection
{
//    unsigned long handler = 0;
//    @synchronized (self.observers) {
//        handler = ((FSFCPClientInfo *)connection).sok;
//        
//        if (ci) {
//            return getEngineInstance()->isConnected(ci.SocketHandler);
//        }
//        return NO;
//    }
    return YES;
}

#pragma --mark private functions
-(void)connect:(long)msgKind ip:(NSString*)ip port:(unsigned int)port timeout:(int)timeout connect:(BOOL)isAsync
{
//    unsigned long handler = 0;
//    char CIP[IP_MAX_LENGTH] = {0};
//    
//    @synchronized (self.observers) {
//        NSString* key = getKey(msgKind);
//        NSObject* _obj = [self.observers objectForKey:key];
//        
//        NSAssert1(_obj, @"[self.observers objectForKey:key] is nil the key: %@", key);
//        
//        FSFCPClientInfo *ci = (FSFCPClientInfo*)_obj;
//        
//        if (ci.SocketHandler) {
//            return;
//        }
//        
//        unsigned long  len = [ip length];
//        if (len < IP_MAX_LENGTH) {
//            memcpy(CIP, [ip UTF8String], len);
//        }else{
//            memcpy(CIP, [ip UTF8String], IP_MAX_LENGTH - 1);
//        }
//        
//        struct hostent * host= NULL;
//        
//        if (ip && [ip length]>0) {
//            host = gethostbyname(CIP);
//        }
//        
//        if ([[UIDevice currentDevice].systemVersion floatValue]>=9.0 && host != NULL && host->h_addrtype == AF_INET6) {
//            ci.SocketHandler = getEngineInstance()->createSocket(AF_Inet6, Socket_Stream);
//        } else {
//            ci.SocketHandler = getEngineInstance()->createSocket(AF_Inet, Socket_Stream);
//        }
//        
//        handler = ci.SocketHandler;
//    }
//    
//    //ci.SocketHandler = getEngineInstance()->createSocket(AF_Inet, Socket_Stream);
////    getEngineInstance()->setSendTimeOut(ci.SocketHandler, 3); //毫秒
//    
//    if (isAsync) {
//        getEngineInstance()->async_connectToHost(handler, CIP, port, msgKind, timeout);
//    }else{
//        getEngineInstance()->connectToHost(handler, CIP, port, msgKind, timeout);
//    }
    
    
}

-(void)addObject:(NSString*)key object:(id)obj
{
//    @synchronized (self.observers) {
//        NSObject* _obj = [self.observers objectForKey:key];
//        if (!_obj) {
//            FSFCPClientInfo* ci = [[FSFCPClientInfo alloc] init];
//            ci.key = key;
//            [ci.clients addObject:obj];
//            [self.observers setObject:ci forKey:key];
//        }else{
//            FSFCPClientInfo *ci = (FSFCPClientInfo*)_obj;
//            [ci.clients addObject:obj];
//        }
//    }
}

-(void)removeObject:(NSString*)key object:(id)obj
{
//    unsigned long handler = 0;
//    @synchronized (self.observers) {
//        NSObject* _obj = [self.observers objectForKey:key];
//        if (_obj) {
//            FSFCPClientInfo *ci = (FSFCPClientInfo*)_obj;
//            if([ci.clients containsObject:obj]){
//                [ci.clients removeObject:obj];
//                if ([ci.clients count] <= 0) {
//                    handler = ci.SocketHandler;
//                    ci.SocketHandler = 0;
//                    ci.key = nil;
//                }
//                [self.observers removeObjectForKey:key];
//            }
//        }
//    }
//    
//    getEngineInstance()->disconnectSocket(handler);
}

@end
