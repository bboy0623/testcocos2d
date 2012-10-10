//
//  GameScene.h
//  DoodleDrop
//
//  Created by jiangyuanli on 12-10-9.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "cocos2d.h"

@interface GameScene : CCLayer {
    CCSprite *player;
    
    CCArray *spiders;
    float spiderMoveDuration;
    int numSpidersMoved;
}

+(id)scene;
//-(id)init;
//-(void)dealloc;

@end
