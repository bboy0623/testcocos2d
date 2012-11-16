#include "HelloWorldScene.h"

#define WINHEIGHT 768
#define WINWIDHT 1024
#define MAX_ENEMY 15
#define MOVESPEED 5

#define KProgressTag 100

using namespace cocos2d;


CCScene* HelloWorld::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        HelloWorld *layer = HelloWorld::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    bool bRet = false;
    do 
    {
        //////////////////////////////////////////////////////////////////////////
        // super init first
        //////////////////////////////////////////////////////////////////////////

        CC_BREAK_IF(! CCLayer::init());

        //////////////////////////////////////////////////////////////////////////
        // add your codes below...
        //////////////////////////////////////////////////////////////////////////

        // 1. Add a menu item with "X" image, which is clicked to quit the program.

        // Create a "close" menu item with close icon, it's an auto release object.
        CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
            "CloseNormal.png",
            "CloseSelected.png",
            this,
            menu_selector(HelloWorld::menuCloseCallback));
        CC_BREAK_IF(! pCloseItem);

        // Place the menu item bottom-right conner.
        pCloseItem->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20));

        // Create a menu with the "close" menu item, it's an auto release object.
        CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
        pMenu->setPosition(CCPointZero);
        CC_BREAK_IF(! pMenu);

        // Add the menu to HelloWorld layer as a child layer.
        this->addChild(pMenu, 1);

        //// 2. Add a label shows "Hello World".
        //// Create a label and initialize with string "Hello World".
        //CCLabelTTF* pLabel = CCLabelTTF::create("Hello World", "Arial", 24);
        //CC_BREAK_IF(! pLabel);
        //// Get window size and place the label upper. 
        //CCSize size = CCDirector::sharedDirector()->getWinSize();
        //pLabel->setPosition(ccp(size.width / 2, size.height - 50));
        //// Add the label to HelloWorld layer as a child layer.
        //this->addChild(pLabel, 1);
        //// 3. Add add a splash screen, show the cocos2d splash image.
        //CCSprite* pSprite = CCSprite::create("HelloWorld.png");
        //CC_BREAK_IF(! pSprite);
        //// Place the sprite on the center of the screen
        //pSprite->setPosition(ccp(size.width/2, size.height/2));
        //// Add the sprite to HelloWorld layer as a child layer.
        //this->addChild(pSprite, 0);

		Energy = 0;
        maxEnergy = 1000;
        
		this->setTouchEnabled(true);
        
        this->LoadTexture();
        
        this->initUITab();
        
        srand(time(NULL));       

		while(this->fishSheet->getChildrenCount() < MAX_ENEMY){
			this->addFish();            
        }
		this->schedule(schedule_selector(HelloWorld::updateGame), 0.05);

        bRet = true;
    } while (0);

    return bRet;
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    // "close" menu item clicked
    CCDirector::sharedDirector()->end();
}

void HelloWorld::LoadTexture()
{
	CCSprite *bg = CCSprite::spriteWithFile("Bg/bj01.jpg");    
	bg->setPosition(ccp(512, 368));
	this->addChild(bg);    
    
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("plist/fish.plist");
	fishSheet = CCSpriteBatchNode::batchNodeWithFile("plist/fish.png");
	this->addChild(fishSheet);    

	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("plist/fish2.plist");
	fish2Sheet = CCSpriteBatchNode::batchNodeWithFile("plist/fish2.png");
	this->addChild(fish2Sheet);    

	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("plist/fish3.plist");
	netSheet = CCSpriteBatchNode::batchNodeWithFile("plist/fish3.png");
	this->addChild(netSheet);    
    
}


void HelloWorld::initUITab()
{

	CCSprite *engryBox = CCSprite::spriteWithFile("Bg/ui_2p_004.png");
	engryBox->setAnchorPoint(ccp(0.5, 0.5));
	engryBox->setPosition(ccp(520,30));
	this->addChild(engryBox);    
    
    engryPointer = CCSprite::spriteWithFile("Bg/ui_2p_005.png");
	engryPointer->setPosition(ccp(520,30));
	this->addChild(engryPointer);    
    
    CCSprite *bgExp = CCSprite::spriteWithFile("Bg/ui_box_01.png");
	bgExp->setPosition(ccp(500, 700));
	this->addChild(bgExp);
    
    CCSprite *bgNum = CCSprite::spriteWithFile("Bg/ui_box_02.png");
    bgNum->setPosition(ccp(440, 90));
	this->addChild(bgNum);
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Bg/cannon.plist");
    cannonSheet = CCSpriteBatchNode::batchNodeWithFile("Bg/cannon.png");
    this->addChild(cannonSheet);
    
    //score1 = [[UIRollNum alloc]init];
	score1 = UIRollNum::create();
    score1->setNumber(10000);
    score1->setPosition(ccp(365, 17));
    this->addChild(score1, 100);
    
    gun = CCSprite::spriteWithSpriteFrameName("Bg/actor_cannon1_71.png");
	gun->setPosition(ccp(520, 50));
    cannonSheet->addChild(gun);
    
    /*添加进度条
     CCProgressTimer *ct=[CCProgressTimer progressWithFile:@"processbanner.png"];  
     ct.position=ccp(512 , 736);  
     ct.percentage=0;
     ct.type=kCCProgressTimerTypeHorizontalBarLR;
     [self addChild:ct z:10 tag:KProgressTag];  
     */  
}

/**************************************************
 scheduleUpdate 的回调函数
 *************************************************/
void HelloWorld::updateGame(float delta)
{
 //   CCFish *sprite;
 //   CCNet *net;
 //   CCScaleTo* scale0 = CCScaleTo::actionWithDuration(0.3, 1.1);
 //   CCScaleTo* scale1 = CCScaleTo::actionWithDuration(0.3, 0.9);
 //   
	//CCARRAY_FOREACH(fishSheet->getChildren(), sprite){
 //       
	//	if (sprite->isCatch()) {
 //           continue;
 //       }
 //       /*
 //       CGPoint pos = sprite.position;
 //       pos.x -= MOVESPEED;
 //       sprite.position = pos;


 //       if(pos.x<-40||pos.y<-40)
 //       {
 //           [fishSheet removeChild:sprite cleanup:NO];
 //       }
 //        */
 //                   //碰撞检测
	//	CCARRAY_FOREACH(fish2Sheet->getChildren() ,net) {
 //           
 //             
	//		if (CCRectContainsPoint(sprite->boundingBox(), net->getPosition())) {
 //            
 //               //if (![sprite randomCatch:sprite.tag]) 
	//			if (!sprite->randomCatch(sprite->getTag())) {
	//				net->setCatching(false);
 //                   break;
 //               }else{
 //                   net->setCatching(false);
	//				sprite->setCatch(true);
 //                   NSMutableArray *fishi01 = [NSMutableArray array];
 //                   for(int i = 1; i <3; i++)
 //                   {
 //                       [fishi01 addObject:
 //                        [[CCSpriteFrameCache sharedSpriteFrameCache] spriteFrameByName:
 //                         [NSString stringWithFormat:@"fish0%d_catch_0%d.png",sprite.tag,i]]];
 //                   }
 //                   
 //                   CCActionInterval *fish01_catch_act = [CCRepeat actionWithAction:[CCAnimate actionWithAnimation:[CCAnimation animationWithFrames:fishi01 delay:0.2f]  restoreOriginalFrame:NO]times:2];
 //                   
 //                   CCSequence* fishSequence = [CCSequence actions:fish01_catch_act,[CCCallFuncO actionWithTarget:self selector:@selector(afterCatch:) object:sprite], nil];
 //                   
 //                   [sprite stopAllActions];
 //                   [sprite runAction:fishSequence];
 //                   
 //                   CCSprite *gold = [CCSprite spriteWithFile:@"+5.png"];
 //                   gold.position =  sprite.position;
 //                   
 //                   CCSequence* goldSequence = [CCSequence actions:scale0, scale1, scale0, scale1,[CCCallFuncO actionWithTarget:self selector:@selector(afterShow:) object:gold], nil];
 //                   [gold runAction:goldSequence];
 //                   [self addChild:gold];
 //               } 
 //           }
 //           
 //           
 //       }
 //             
	//}
 //   
 //   CCARRAY_FOREACH([fish2Sheet children],net)
 //   {
 //       if ([net isCatching]) {
 //           continue;
 //       } 

 //       [fish2Sheet removeChild:net cleanup:NO];
 //   
 //       CCNet *tapnet = [CCNet spriteWithSpriteFrameName:@"net01.png"];
 //       tapnet.position = net.position;
 //       CCSequence* netSequence = [CCSequence actions:scale0, scale1, scale0, scale1,[CCCallFuncO actionWithTarget:self selector:@selector(afterShowNet:) object:tapnet], nil];
 //   
 //       [tapnet runAction:netSequence];
 //       [netSheet addChild:tapnet];                    
 //   
 //       [score1 setNumber:([score1 getNumber]+5)];
 //   
 //   
 //   }
 //   
 //   
 //   while ([[fishSheet children]count]<MAX_ENEMY)
 //   {
 //       [self addFish];
 //   }

	
}

void HelloWorld::updateEnergry(int en)
{
    Energy += en;
    if (Energy>=maxEnergy) {
        Energy = maxEnergy;
    }
    float rotation = 180.0 * Energy/maxEnergy;
	engryPointer->setRotation(rotation);
}

void HelloWorld::addFish()
{
    int type = rand()%8+1;

	CCArray *fishi01 = CCArray::create();			
    for(int i = 1; i <10; i++) {
        fishi01->addObject(
			CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(
				CCString::stringWithFormat("fish0%d_0%d.png", type, i)->getCString()
			)
		);
    }
        
	//fish01_act = [CCRepeatForever actionWithAction:[CCAnimate actionWithAnimation:[CCAnimation animationWithFrames:fishi01 delay:0.2f] restoreOriginalFrame:YES]];
	//TODO actionWithAnimation?
    fish01_act = CCRepeatForever::actionWithAction(
		CCAnimate::actionWithAnimation(
			CCAnimation::animationWithSpriteFrames(fishi01, 0.2f)
		)
	);
   
	//CCFish *fish = [CCFish spriteWithSpriteFrameName: [NSString stringWithFormat:@"fish0%d_0%d.png",type,1]];
	CCFish *fish = CCFish::create();
	fish->spriteWithSpriteFrameName(CCString::stringWithFormat("fish0%d_0%d.png", type, 1)->getCString());
                               
	fish->setScale(1.2f);
	fish->setTag(type);
	fish->setCatch(false);
    fish->runAction(fish01_act);
    fish->addPath();
    fishSheet->addChild(fish);
    
}


void HelloWorld::ccTouchesBegan(CCSet *touches, CCEvent *event)
{
	//for (CCTouch *touch in touches){
	for (CCSetIterator iter = touches->begin(); iter != touches->end(); iter++){    
		CCTouch *touch = (CCTouch *)*iter;	
        //CGPoint pos = [touch locationInView:touch.view];
		CCPoint pos = touch->locationInView();
		pos = CCDirector::sharedDirector()->convertToGL(pos);
        gun->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("actor_cannon1_72.png"));
        
		float angle = (pos.y - gun->getPosition().y)/(pos.x-gun->getPosition().x);
        angle = atanf(angle)/M_PI*180;
        if (angle<0) {
			gun->setRotation(-(90+angle));
        }else if (angle>0)
        {
			gun->setRotation(90 - angle);
        }
    }
}

void HelloWorld::ccTouchesEnded(CCSet *touches, CCEvent *event)
{
    //for (UITouch *touch in touches){
	for (CCSetIterator iter = touches->begin(); iter != touches->end(); iter++){    
		CCTouch *touch = (CCTouch *)*iter;	

        gun->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("actor_cannon1_71.png"));
        //CGPoint pos = [touch locationInView:touch.view];
		CCPoint pos = touch->locationInView();
		pos = CCDirector::sharedDirector()->convertToGL(pos);
        
        score1->setNumber(score1->getNumber()-rand()%20-2);
      
		CCNet *labelboard = CCNet::create();
        labelboard->spriteWithSpriteFrameName("bullet01.png");
		labelboard->setPosition(ccp(512, 50));
		labelboard->setCatching(true);
		CCMoveTo *move = CCMoveTo::actionWithDuration(1.0, pos);		

		//CCSequence* netSequence = [CCSequence actions:move,[CCCallFuncO actionWithTarget:self selector:@selector(ShowNet:) object:labelboard], nil];
		CCFiniteTimeAction *netSequence = CCSequence::actions(move, CCCallFuncO::actionWithTarget(this, callfuncO_selector(HelloWorld::ShowNet), labelboard), NULL);
        
		labelboard->setRotation(gun->getRotation());
        labelboard->runAction(netSequence);
        fish2Sheet->addChild(labelboard);
                
        this->updateEnergry(rand()%20);
    }
}

void HelloWorld::ShowNet(CCObject* sender)
{
    CCSprite *sp = (CCSprite *)sender;
 
    fish2Sheet->removeChild(sp, false);
    
    sp->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("net01.png"));
 
    
    CCScaleTo* scale0 = CCScaleTo::actionWithDuration(0.3, 1.1);
    CCScaleTo* scale1 = CCScaleTo::actionWithDuration(0.3, 0.9);
    
    //CCSequence* netSequence = [CCSequence actions:scale0, scale1, scale0, scale1,[CCCallFuncO actionWithTarget:self selector:@selector(afterShowNet:) object:sp], nil];
	CCFiniteTimeAction * netSequence = CCSequence::actions(scale0, scale1, scale0, scale1, CCCallFuncO::actionWithTarget(this, callfuncO_selector(HelloWorld::afterShowNet), sp), NULL);
    
    sp->runAction(netSequence);
    netSheet->addChild(sp);
}

void HelloWorld::afterShowNet(CCObject *sender)
{
    CCSprite *sp = (CCSprite *)sender;
    netSheet->removeChild(sp, false);
}

void HelloWorld::afterShow(CCObject *sender)
{
    CCSprite *sp = (CCSprite *)sender;
    this->removeChild(sp, false);
}

void HelloWorld::afterCatch(CCSprite* sender)
{       
    CCSprite *sp = sender;
    fishSheet->removeChild(sp, false);
}



//// on "dealloc" you need to release all your retained objects
//- (void) dealloc
//{
//	// in case you have something to dealloc, do it in this method
//	// in this particular example nothing needs to be released.
//	// cocos2d will automatically release all the children (Label)
//	
//	// don't forget to call "super dealloc"
//	[super dealloc];
//}

