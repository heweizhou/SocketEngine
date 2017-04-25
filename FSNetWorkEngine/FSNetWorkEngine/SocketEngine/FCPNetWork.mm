//
//  FCPNetWork.m
//  FCPNetWork
//
//  Created by 周贺伟 on 15/12/22.
//  Copyright © 2015年 周贺伟. All rights reserved.
//

#import "FCPNetWork.h"
#import "IFSNetworkEngine.h"
#import "SocketDefines.h"
#import "ObjcCallBack.h"

@interface FSClientInfo  : NSObject
{
    
}

@property(nonatomic, copy) NSString* key;
@property(nonatomic, assign) package_param* param;
@property(nonatomic, assign) unsigned long SocketHandler;
@property(nonatomic, strong) NSMutableArray* clients;
@end

@implementation FSClientInfo

-(id)init
{
    self = [super init];
    if (self) {
        self.key = nil;
        self.SocketHandler = 0;
        
        self.param = new package_param;
        self.param->msg_kind = 0;
        self.param->package_param = 0;

        self.clients = [[NSMutableArray alloc] init];
    }
    
    return self;
}

-(void)dealloc
{
    delete self.param;
}

@end

@interface FCPNetWork()
@property(nonatomic, strong) NSMutableDictionary* observers;
@property(nonatomic, assign) ObjcCallBack* CPPCallBack;
@end

@implementation FCPNetWork


+ (FCPNetWork *)sharedNetWorkManager
{
    static FCPNetWork *sharedInstance = nil;
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
        self.CPPCallBack = new ObjcCallBack();
        getEngineInstance()->setCallBack(self.CPPCallBack);
    }
    
    return self;
}

-(void)dealloc
{
    delete self.CPPCallBack;
}

-(void)registerSokcet:(id)obj ip:(NSString*)ip port:(unsigned int)port timeout:(int)timeout msgKind:(long)kind selector:(SEL)selector connectMethodIsAsync:(BOOL)isAsync
{
    NSNotificationCenter *center = [NSNotificationCenter defaultCenter];
    NSString* key = getKey(kind);
    
    [self addObject:key object:obj];
    [self connect:kind ip:ip port:port timeout:timeout connect:isAsync];
    
    [center addObserver:obj selector:selector name:key object:nil];
    
}

-(void)unRegisterSokcet:(id)obj msgKind:(long)kind;
{
    NSString* key = getKey(kind);
    NSNotificationCenter *center = [NSNotificationCenter defaultCenter];
    [center removeObserver:obj name:key object:nil];
    [self removeObject:key object:obj];
}

-(void)async_send:(long)msgKind buffer:(NSData*)data param:(id)param
{
    NSString* key = getKey(msgKind);
    NSObject* _obj = [self.observers objectForKey:key];
    if (_obj) {
        FSClientInfo *ci = (FSClientInfo*)_obj;
        if (ci.SocketHandler) {
            
            ci.param->msg_kind = msgKind;
            ci.param->package_param = (long)(__bridge void*)param;
            
            getEngineInstance()->async_sendData(ci.SocketHandler, (const char*)[data bytes], [data length], (void*)ci.param, sizeof(package_param));
        }
    }
    NSAssert1(_obj, @"[self.observers objectForKey:key] is nil the key: %@", key);
}

- (BOOL)isConnected:(long)msgKind
{
    NSString *key = getKey(msgKind);
    FSClientInfo *ci = self.observers[key];
    return getEngineInstance()->isConnected(ci.SocketHandler);
}

#pragma --mark private functions
-(void)connect:(long)msgKind ip:(NSString*)ip port:(unsigned int)port timeout:(int)timeout connect:(BOOL)isAsync
{
    NSString* key = getKey(msgKind);
    NSObject* _obj = [self.observers objectForKey:key];

    NSAssert1(_obj, @"[self.observers objectForKey:key] is nil the key: %@", key);

    FSClientInfo *ci = (FSClientInfo*)_obj;
    
    if (ci.SocketHandler) {
        return;
    }
    
    ci.SocketHandler = getEngineInstance()->createSocket(AF_Inet, Socket_Stream);
    getEngineInstance()->setSendTimeOut(ci.SocketHandler, timeout*1000); //毫秒
    if (isAsync) {
        ci.param->msg_kind = msgKind;
        getEngineInstance()->async_connectToHost(ci.SocketHandler, [ip UTF8String], port, (void*)ci.param, sizeof(package_param));
    }else{
        getEngineInstance()->connectToHost(ci.SocketHandler, [ip UTF8String], port);
    }
    
}
-(void)addObject:(NSString*)key object:(id)obj
{
    NSObject* _obj = [self.observers objectForKey:key];
    if (!_obj) {
        FSClientInfo* ci = [[FSClientInfo alloc] init];
        ci.key = key;
        [ci.clients addObject:obj];
        [self.observers setObject:ci forKey:key];
    }else{
        FSClientInfo *ci = (FSClientInfo*)_obj;
        [ci.clients addObject:obj];
    }

}

-(void)removeObject:(NSString*)key object:(id)obj
{
    NSObject* _obj = [self.observers objectForKey:key];
    if (_obj) {
        FSClientInfo *ci = (FSClientInfo*)_obj;
        if([ci.clients containsObject:obj]){
            [ci.clients removeObject:obj];
            if ([ci.clients count] <= 0) {
                getEngineInstance()->disconnectSocket(ci.SocketHandler);
                ci.SocketHandler = 0;
                ci.key = nil;
            }
            [self.observers removeObjectForKey:_obj];
//            if ([self.observers count] <= 0) {
//                destorySocketEngineInstance();
//            }
        }
    }
}

@end
