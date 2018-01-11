//
//  Channel.m
//  FSNetWorkEngine
//
//  Created by 周贺伟 on 16/5/23.
//  Copyright © 2016年 周贺伟. All rights reserved.
//

#import "Channel.h"
#import "Channel+Sender.h"
#import "TaskBean+Private.h"

@interface BeanPackage : NSObject

@property(nonatomic, strong) id         bean;
@property(nonatomic, assign) int64_t    tag;
@property(nonatomic, assign) int        timeout;

@end

@implementation BeanPackage

-(instancetype)initWithBean:(TaskBean*)bean;
{
    self = [super init];
    if (self) {
        self.bean = bean;
        self.timeout = bean.timeout;
        self.tag = bean.tag;
    }
    
    return self;
}

@end

//channel

@interface Channel()
@property (nonatomic, weak)     id<ChannelSendDelegate>         beanSendDelegate;

@property(nonatomic, strong)    dispatch_source_t               timer;
@property(nonatomic, strong)    NSMutableDictionary*            normal_task_timeout_dic;
@end

@implementation Channel


#pragma  -- mark life cycle
- (instancetype)init
{
    self = [super init];
    if (self) {
        _normal_task_timeout_dic = [[NSMutableDictionary alloc] init];
        [self initTimer];
    }
    
    return self;
}

- (instancetype)initWithDelegate:(id<ChannelStatuesDelegate>)statusDelegate;
{
    self = [super init];
    if (self) {
        _normal_task_timeout_dic = [[NSMutableDictionary alloc] init];
        
        self.statuesDelegate = statusDelegate;
        
        [self initTimer];
    }
    
    return self;
}

#pragma --mark sendDelegate
- (void)setSendDelegate:(id<ChannelSendDelegate>)sendDelegate
{
    self.beanSendDelegate = sendDelegate;
}

-(BOOL)beforeSend:(Channel*)channel bean:(TaskBean*)bean
{
    if (self.beanSendDelegate) {
        if ([self.beanSendDelegate respondsToSelector:@selector(beforeTaskSend:bean:)]) {
            return [self.beanSendDelegate beforeTaskSend:channel bean:bean];
        }
    }
    return NO;
}

-(BOOL)sendTask:(Channel*)channel bean:(TaskBean*)bean HighPriority:(BOOL)HP
{
    if (self.beanSendDelegate) {
        if ([self.beanSendDelegate respondsToSelector:@selector(sendTask:bean:HighPriority:)]) {
            return [self.beanSendDelegate sendTask:channel bean:bean HighPriority:HP];
        }
    }
    return NO;
}

-(void)afterSend:(Channel*)channel bean:(TaskBean*)bean
{
    if (self.beanSendDelegate) {
        if ([self.beanSendDelegate respondsToSelector:@selector(afterTaskSend:bean:)]) {
            [self.beanSendDelegate afterTaskSend:channel bean:bean];
        }
    }
}

-(BOOL)cancelSendTask:(Channel*)channel bean:(TaskBean*)bean
{
    if (self.beanSendDelegate) {
        if ([self.beanSendDelegate respondsToSelector:@selector(cancelTask:bean:)]) {
            return [self.beanSendDelegate cancelTask:self bean:bean];
        }
    }
    
    return NO;
}

#pragma --mark statusDelegate
- (void)setStatusDelegate:(id<ChannelStatuesDelegate>)statusDelegate
{
    self.statuesDelegate = statusDelegate;
}

- (void)sendWithBean:(TaskBean*)bean HighPriority:(BOOL)HP
{
    if ([self beforeSend:self bean:bean])
    {
        [bean setChannel:self];
        [self addToBeanManager:bean];
        [self sendTask:self bean:bean HighPriority:HP];
        [self afterSend:self bean:bean];
    }else{
        [self _onDataLinkInvalid:bean];
    }
    
}

- (void)cancelTask:(TaskBean*)bean
{
    [self removeTaskBean:bean];
    [self cancelSendTask:self bean:bean];
}

- (void)cancelAllTask
{
    @synchronized (self.normal_task_timeout_dic){
        [self _onTaskCancel:[self.normal_task_timeout_dic allValues]];
        [_normal_task_timeout_dic removeAllObjects];
    }
}

-(void)dealloc
{
    [self clearTimer];
    [self cancelAllTask];
}

#pragma -- mark private function

-(void)initTimer
{
    if (![self enableTaskManage]) {
        return ;
    }
    
    dispatch_queue_t queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
    double interval = 1.000f;
    if (!_timer)
    {
        _timer = dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER, 0, 0, queue);
        dispatch_source_set_timer(_timer, dispatch_time(DISPATCH_TIME_NOW, 0), interval * NSEC_PER_SEC, 0);
        dispatch_source_set_event_handler(_timer, ^{
            [self timerAction];
        });
        dispatch_resume(_timer);
    }
    
}

-(void)clearTimer
{
    if (_timer) {
        dispatch_source_cancel(_timer);
        _timer = nil;
    }
}

- (void)timerAction
{
    
    [self taskBeanTimeoutCheck:self.normal_task_timeout_dic];
    if (self) {
        if ([self.statuesDelegate respondsToSelector:@selector(on_timer_action)]) {
            [self.statuesDelegate on_timer_action];
        }
    }
}

-(void)taskBeanTimeoutCheck:(NSMutableDictionary*)dic
{
    NSMutableArray* removeIndex_arr = [[NSMutableArray alloc] init];
    
    @synchronized (dic) {
        if ([dic count] <= 0) {
            return ;
        }
        
        [dic enumerateKeysAndObjectsUsingBlock:^(id  _Nonnull key, id  _Nonnull obj, BOOL * _Nonnull stop) {
            BeanPackage* bp = (BeanPackage*)obj;
            bp.timeout--;
            if (bp.timeout <= 0) {
                [removeIndex_arr addObject:bp.bean];
            }
        }];
    }
    
    if ([removeIndex_arr count] > 0) {
        [removeIndex_arr enumerateObjectsUsingBlock:^(id  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
            [self _onBeanTimeout:obj];
        }];
    }
    
}

-(void)addToBeanManager:(TaskBean*)bean
{
    if (![self enableTaskManage]) {
        return ;
    }
    
    NSString* key = [[NSString alloc] initWithFormat:@"%lld", bean.tag];
    BeanPackage* beanPackage = [[BeanPackage alloc] initWithBean:bean];
    @synchronized (self.normal_task_timeout_dic){
        [_normal_task_timeout_dic setObject:beanPackage forKey:key];
    }
}

-(void)removeTaskBean:(TaskBean*)bean
{
    if (bean) {
        NSString* key = [[NSString alloc] initWithFormat:@"%lld", bean.tag];
        @synchronized (self.normal_task_timeout_dic) {
            [_normal_task_timeout_dic removeObjectForKey:key];
        }
    }
}

- (TaskBean*)getTaskRefByTag:(int64_t)tag
{
    NSString* key = [[NSString alloc] initWithFormat:@"%lld", tag];
    TaskBean* bean = nil;
    
    @synchronized (self.normal_task_timeout_dic) {
        BeanPackage* bp = [_normal_task_timeout_dic valueForKey:key];
        bean = bp.bean;
        bp.timeout = bean.timeout;
    }
    
    
    return bean;
}


#pragma  -- mark delegate

- (void)_onDataLinkInvalid:(TaskBean*)bean
{
    if (self) {
        if ([self.taskDelegate respondsToSelector:@selector(onDataLinkInvalid:bean:)]) {
            [self.taskDelegate onDataLinkInvalid:self bean:bean];
        }
    }
}

- (void)_onTaskCancel:(NSArray<BeanPackage*>*)UnDoneBeanPackages
{
    if (self) {
        if ([self.taskDelegate respondsToSelector:@selector(onTaskCancel:)]) {
            
            NSMutableArray* UnDonebeans = [[NSMutableArray alloc] init];
            [UnDoneBeanPackages enumerateObjectsUsingBlock:^(BeanPackage * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
                [UnDonebeans addObject:obj.bean];
            }];
            
            [self.taskDelegate onTaskCancel:UnDonebeans];
        }
    }
}

- (void)_onBeanTimeout:(id)bean
{
    [self removeTaskBean:bean];
    [self cancelSendTask:self bean:bean];
    if (self) {
        if ([self.taskDelegate respondsToSelector:@selector(onBeanTimeout:channel:)]) {
            [self.taskDelegate onBeanTimeout:bean channel:self];
        }
    }
}

#pragma  -- mark override

- (void)processMsg:(id)msg bean:(TaskBean*)bean;
{
    if (self.taskDelegate) {
        if ([self.taskDelegate respondsToSelector:@selector(onProcessMsg:channel:withMsg:)]){
            if([self.taskDelegate onProcessMsg:bean channel:self withMsg:msg]){
                [self removeTaskBean:bean];
            }
        }
    }
}

-(BOOL)enableTaskManage
{
    return YES;
}

#pragma  -- mark channel private function to Session
- (void)sendSuccess:(TaskBean*)bean
{
    if (self) {
        if ([self.taskDelegate respondsToSelector:@selector(sendSuccess:channel:)]) {
            [self.taskDelegate sendSuccess:bean channel:self];
        }
    }
}
- (void)sendFailed:(TaskBean*)bean
{
//    [self sendWithBean:bean HighPriority:YES];
    
    if (self) {
        if ([self.taskDelegate respondsToSelector:@selector(sendFailed:channel:)]) {
            [self.taskDelegate sendFailed:bean channel:self];
        }
    }
}
- (void)sendPackageInvalid:(TaskBean*)bean
{
    [self removeTaskBean:bean];
    if (self) {
        if ([self.taskDelegate respondsToSelector:@selector(sendPackageInvalid:channel:)]) {
            [self.taskDelegate sendPackageInvalid:bean channel:self];
        }
    }
}

- (BOOL)isHavePenddingTask
{
    if (self.statuesDelegate) {
        if ([self.statuesDelegate respondsToSelector:@selector(is_have_pedding_task)]) {
            return [self.statuesDelegate is_have_pedding_task];
        }
    }
    
    return NO;
}

-(void)onChannelInit
{
    if (self.statuesDelegate) {
        if ([self.statuesDelegate respondsToSelector:@selector(on_channel_init)]) {
            [self.statuesDelegate on_channel_init];
        }
    }

}

-(void)onSyncConnectionInvalid
{
    if (self.statuesDelegate) {
        if ([self.statuesDelegate respondsToSelector:@selector(on_connect_invalid)]) {
            [self.statuesDelegate on_connect_invalid];
        }
    }
}

@end
