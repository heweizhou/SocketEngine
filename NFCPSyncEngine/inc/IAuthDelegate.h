//
//  IAuthDelegate.h
//  NFCPSyncEngine
//
//  Created by Lin on 16/6/6.
//  Copyright © 2016年 fxiaoke. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Channel.h"

@protocol IAuthDelegate <NSObject>

-(void)onLinkSetup:(Channel*) channel parameters:(NSDictionary*) parameters;
-(void)onLinkSetupSuccessed:(Channel*) channel parameters:(NSDictionary*) parameters;
-(void)onLinkSetupFailed:(Channel*) channel parameters:(NSDictionary*) parameters;
-(void)onLinkSetupTimeout:(Channel*) channel parameters:(NSDictionary*) parameters;

-(void)onAuthSetup:(Channel*) channel parameters:(NSDictionary*) parameters;
-(void)onAuthSuccessed:(Channel*) channel parameters:(NSDictionary*) parameters;
-(void)onAuthFailed:(Channel*) channel parameters:(NSDictionary*) parameters;
-(void)onAuthTimeout:(Channel*) channel parameters:(NSDictionary*) parameters;

@end
