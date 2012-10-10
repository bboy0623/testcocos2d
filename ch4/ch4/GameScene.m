//
//  GameScene.m
//  DoodleDrop
//
//  Created by jiangyuanli on 12-10-9.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#import "GameScene.h"
#import "libs/CocosDenshion/SimpleAudioEngine.h"

@interface GameScene ()

-(void)initSpiders;
-(void)resetSpiders;
-(void)updateSpiders;
-(void)runSpider:(CCSprite *)spider;
-(void)spiderBelowScreen:(id)sender;

-(void)checkForCollision;

@end

@implementation GameScene

+(id)scene
{
    CCScene * scene = [CCScene node];
    
    CCLayer *layer = [GameScene node];
    
    [scene addChild:layer];
    
    return scene;
}

//初始化
-(id)init
{
    if (!(self = [super init])){
        return self;
    }
    
    CCLOG(@"init");
        
    self.isAccelerometerEnabled = true;
        
    player = [CCSprite spriteWithFile:@"alien.png"];
    [self addChild:player z:0 tag:1];
        
    CGSize winSize = [[CCDirector sharedDirector] winSize];

    //player.position = CGPointMake(winSize.width/2, [player texture].contentSize.height/2);
        
        
    [self scheduleUpdate];
    [self initSpiders];
    
    scoreLabel = [CCLabelTTF labelWithString:@"0" fontName:@"Arial" fontSize:32];
    scoreLabel.position = CGPointMake(winSize.width/2, winSize.height);
    scoreLabel.anchorPoint = CGPointMake(0.5, 1);
    [self addChild:scoreLabel z:-1];
    
    //audio
    //[[SimpleAudioEngine sharedEngine]playBackgroundMusic:@"blues.mp3" loop:YES];
    //[[SimpleAudioEngine sharedEngine]playEffect:@"alien-sfx.caf"];
    
    return self;    
}

-(void)dealloc
{
    CCLOG(@"dealloc");
    
    [spiders release];
    spiders = nil;
    
    [super dealloc];
}

//初始化蜘蛛
-(void)initSpiders
{
    CCLOG(@"initSpiders");
    
    CGSize winSize = [[CCDirector sharedDirector] winSize];
    
    CCSprite *tmpSpire = [CCSprite spriteWithFile:@"spider.png"];
    float spiderWidth = [[tmpSpire texture] contentSize].width;
    
    //蜘蛛个数
    int numSpiders = winSize.width / spiderWidth;
    //int numSpiders = 1;
    
    spiders = [[CCArray alloc] initWithCapacity:numSpiders];
    
    for (int i=0; i<numSpiders; i++){
        CCSprite *spider = [CCSprite spriteWithFile:@"spider.png"];
        //
        spider.position = CGPointMake(60, 60);
        
        [spiders addObject:spider];
        
        [self addChild:spider z:0 tag:2];
        
    }
    
    [self resetSpiders];
}

//重置蜘蛛位置和action
-(void)resetSpiders
{
    CGSize winSize = [[CCDirector sharedDirector] winSize];
    
    CGSize spiderSize = [[spiders lastObject] texture].contentSize;
    
    for (int i=0; i<[spiders count]; i++){
        CCSprite *spider = [spiders objectAtIndex:i];
        
        spider.position = CGPointMake(spiderSize.width*i+spiderSize.width*0.5f, winSize.height - spiderSize.height);
        
        //CCLOG(@"spider position %f, %f", spider.position.x, spider.position.y);
        
        [spider stopAllActions];
    }
    
    [self unschedule:@selector(updateSpiders)];
    
    [self schedule:@selector(updateSpiders) interval:0.7];
    
    numSpidersMoved = 0;
    spiderMoveDuration = 4.0;
    
    totalTime = 0.0;
    score = 0;
}

//选择一个未运动的蜘蛛，运动
-(void)updateSpiders
{
    for (int i=0; i<10; i++){
        CCSprite *randSpider = [spiders objectAtIndex:CCRANDOM_0_1() * [spiders count]];
        
        if ([randSpider numberOfRunningActions] == 0){
            [self runSpider:randSpider];
            
            break;
        }
    }
}

//蜘蛛动作
-(void)runSpider:(CCSprite *)spider
{
    numSpidersMoved++;
    
    if (numSpidersMoved % 8 == 0 && spiderMoveDuration > 2.0){
        spiderMoveDuration -= 0.1;
    }
    
    CGPoint belowScreenPos = CGPointMake(spider.position.x, -[spider texture].contentSize.height);
    
    CCMoveTo *move = [CCMoveTo actionWithDuration:spiderMoveDuration position:belowScreenPos];
    
    CCCallFuncN *call = [CCCallFuncN actionWithTarget:self selector:@selector(spiderBelowScreen:)];
    
    CCSequence *seqact = [CCSequence actions:move, call, nil];
    
    [spider runAction:seqact];
}

//动作结束后，设置位置
-(void)spiderBelowScreen:(id)sender
{
    
    //NSASSert([sender isKindOfClass:[CCSprite class]], @"sender is not CCSpire");
    
    CCSprite *spider = (CCSprite *)sender;
    
    CGPoint pos = spider.position;    
    
    pos.y = [[CCDirector sharedDirector] winSize].height - [spider texture].contentSize.height;
    
    spider.position = pos;
    
}

//检查冲突
-(void)checkForCollision
{
    float playerSize = [player texture].contentSize.width;
    float spiderSize = [[spiders lastObject] texture].contentSize.width;
    
    float playerRadius = 0.4 * playerSize;
    float spiderRadius = 0.4 * spiderSize;
    
    float maxCollisionDistance = playerRadius + spiderRadius;
    
    for (int i=0; i<[spiders count]; i++){
        CCSprite *spider = [spiders objectAtIndex:i];
        
        if ([spider numberOfRunningActions] == 0){
            continue;
        }
        
        float actualDistance = ccpDistance(player.position, spider.position);
        
        if (actualDistance < maxCollisionDistance){
            //CCLOG(@"u bit by spider %i", i);
            [self resetSpiders];
        }
    }
}

-(void)update:(ccTime)delta
{
    //CCLOG(@"update");
    [self checkForCollision];
    
    totalTime += delta;
    int curTime = (int)totalTime;
    if (score < curTime){
        score = curTime;
        [scoreLabel setString:[NSString stringWithFormat:@"%i", score]];
    }
}

-(void)onEnter
{
    CCLOG(@"GameScene onEnter");
    [super onEnter];
}

-(void)onExitTransitionDidStart
{
    CCLOG(@"GameScene onExitTransitionDidStart");
    [super onExitTransitionDidStart];
}

-(void)onEnterTransitionDidFinish
{
    CCLOG(@"GameScene onEnterTransitionDidFinish");
    [super onEnterTransitionDidFinish];
}

-(void)onExit
{
    CCLOG(@"GameScene onExit");
    [super onExit];
}


@end
