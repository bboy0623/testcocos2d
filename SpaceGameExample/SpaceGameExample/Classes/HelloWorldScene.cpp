#include "HelloWorldScene.h"

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

		_batchNode = CCSpriteBatchNode::batchNodeWithFile("Sprites.pvr.ccz");
		this->addChild(_batchNode);
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Sprites.plist");
 
		_ship = CCSprite::spriteWithSpriteFrameName("SpaceFlier_sm_1.png");
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		_ship->setPosition(ccp(winSize.width * 0.1, winSize.height * 0.5));
		_batchNode->addChild(_ship, 1);

		// 1) Create the CCParallaxNode
		_backgroundNode = CCParallaxNodeExtras::node() ; //1 
		this->addChild(_backgroundNode,-1) ;
 
		// 2) Create the sprites we'll add to the CCParallaxNode
		_spacedust1			= CCSprite::spriteWithFile("bg_front_spacedust.png");
		_spacedust2			= CCSprite::spriteWithFile("bg_front_spacedust.png");
		_planetsunrise		= CCSprite::spriteWithFile("bg_planetsunrise.png");
		_galaxy				= CCSprite::spriteWithFile("bg_galaxy.png");
		_spacialanomaly		= CCSprite::spriteWithFile("bg_spacialanomaly.png");
		_spacialanomaly2	= CCSprite::spriteWithFile("bg_spacialanomaly2.png"); 
 
		// 3) Determine relative movement speeds for space dust and background
		CCPoint dustSpeed = ccp(0.1, 0.1);
		CCPoint bgSpeed = ccp(0.05, 0.05);
 
		// 4) Add children to CCParallaxNode
		_backgroundNode->addChild(_spacedust1, 0 , dustSpeed , ccp(0,winSize.height/2) ); // 2
		_backgroundNode->addChild(_spacedust2, 0 , dustSpeed , ccp( _spacedust1->getContentSize().width,winSize.height/2)); 
		_backgroundNode->addChild(_galaxy,-1, bgSpeed , ccp(0,winSize.height * 0.7));
		_backgroundNode->addChild(_planetsunrise,-1 , bgSpeed,ccp(600,winSize.height * 0));   
		_backgroundNode->addChild(_spacialanomaly,-1, bgSpeed,ccp(900,winSize.height * 0.3));        
		_backgroundNode->addChild(_spacialanomaly2,-1, bgSpeed,ccp(1500,winSize.height * 0.9));

		//ÃÌº”–«–«
		HelloWorld::addChild(CCParticleSystemQuad::particleWithFile("Stars1.plist")) ;
		HelloWorld::addChild(CCParticleSystemQuad::particleWithFile("Stars2.plist")) ;
		HelloWorld::addChild(CCParticleSystemQuad::particleWithFile("Stars3.plist")) ;

		//accelerate
		this->setAccelerometerEnabled(false);

		//¥¥Ω®‘… Ø
		#define KNUMASTEROIDS 15
		_asteroids = new CCArray(); 
		for(int i = 0; i < KNUMASTEROIDS; ++i) {
			CCSprite *asteroid = CCSprite::spriteWithSpriteFrameName("asteroid.png");			
			asteroid->setVisible(false);
			_batchNode->addChild(asteroid);
			_asteroids->addObject(asteroid);
		}
		_nextAsteroid = 0;

		//º§π‚
		#define KNUMLASERS 5
		_shipLasers = new CCArray(); 
		for(int i = 0; i < KNUMLASERS; ++i) {
			CCSprite *shipLaser = CCSprite::spriteWithSpriteFrameName("laserbeam_blue.png");
			shipLaser->setVisible(false) ;
			_batchNode->addChild(shipLaser);
			_shipLasers->addObject(shipLaser);
		}
		_nextShipLaser = 0;
		this->setTouchEnabled(true) ;

		// §¿˚/ ß∞‹ºÏ≤‚
		_gameOver = false;
		_lives = 3 ;
		double curTime = getTimeTick() ;
		_gameOverTime = curTime + 30000 ;

		//music
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic("SpaceGame.wav",true) ;
		SimpleAudioEngine::sharedEngine()->preloadEffect("explosion_large.wav") ;
		SimpleAudioEngine::sharedEngine()->preloadEffect("laser_ship.wav") ;

		//update
		this->scheduleUpdate();

        bRet = true;
    } while (0);

    return bRet;
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    // "close" menu item clicked
    CCDirector::sharedDirector()->end();
}

void HelloWorld::update (float dt)
{
	CCPoint backgroundScrollVert = ccp(-1000,0) ;
	_backgroundNode->setPosition(ccpAdd(_backgroundNode->getPosition(),ccpMult(backgroundScrollVert,dt))) ; 


	CCArray *spaceDusts = CCArray::arrayWithCapacity(2) ;
	spaceDusts->addObject(_spacedust1) ;
	spaceDusts->addObject(_spacedust2) ;
	for ( int ii = 0  ; ii <spaceDusts->count() ; ii++ ) {
		CCSprite * spaceDust = (CCSprite *)(spaceDusts->objectAtIndex(ii)) ;
		float xPosition = _backgroundNode->convertToWorldSpace(spaceDust->getPosition()).x  ;
		float size = spaceDust->getContentSize().width ;
		if ( xPosition < -size ) {
			_backgroundNode->incrementOffset(ccp(spaceDust->getContentSize().width*2,0),spaceDust) ; 
		}                                   
	}
 
	CCArray *backGrounds = CCArray::arrayWithCapacity(4) ;
	backGrounds->addObject(_galaxy) ;
	backGrounds->addObject(_planetsunrise) ;
	backGrounds->addObject(_spacialanomaly) ;
	backGrounds->addObject(_spacialanomaly2) ;
	for ( int ii = 0 ; ii <backGrounds->count() ; ii++ ) {
		CCSprite * background = (CCSprite *)(backGrounds->objectAtIndex(ii)) ;
		float xPosition = _backgroundNode->convertToWorldSpace(background->getPosition()).x ;
		float size = background->getContentSize().width ;
		if ( xPosition < -size ) {
			_backgroundNode->incrementOffset(ccp(2000,0),background) ; 
		}
	}

	//accelerate
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
#if 0
	float maxY = winSize.height - _ship->getContentSize().height/2;
	float minY = _ship->getContentSize().height/2;
 
	float diff = (_shipPointsPerSecY * dt) ;
	float newY = _ship->getPosition().y + diff;
	newY = MIN(MAX(newY, minY), maxY);
	_ship->setPosition(ccp(_ship->getPosition().x, newY));
#endif

	//‘… Ø
	float curTimeMillis = getTimeTick();
	if (curTimeMillis > _nextAsteroidSpawn) {
 
		float randMillisecs = randomValueBetween(0.20,1.0) * 1000;
		_nextAsteroidSpawn = randMillisecs + curTimeMillis;
 
		float randY = randomValueBetween(0.0,winSize.height);
		float randDuration = randomValueBetween(2.0,10.0);
 
		CCSprite *asteroid = (CCSprite *)_asteroids->objectAtIndex(_nextAsteroid);
		_nextAsteroid++;
 
		if (_nextAsteroid >= _asteroids->count())
			_nextAsteroid = 0;
 
		asteroid->stopAllActions();
		asteroid->setPosition( ccp(winSize.width+asteroid->getContentSize().width/2, randY));
		asteroid->setVisible(true) ;
		asteroid->runAction ( CCSequence::actions (
			CCMoveBy::actionWithDuration(randDuration,ccp(-winSize.width-asteroid->getContentSize().width,0)) , 
			CCCallFuncN::actionWithTarget(this,callfuncN_selector(HelloWorld::setInvisible)) ,
			NULL // DO NOT FORGET TO TERMINATE WITH NULL (unexpected in C++)
			) ) ;        
	}

	//≈ˆ◊≤ºÏ≤‚
	// Asteroids
	CCObject* itAster;
	CCARRAY_FOREACH(_asteroids, itAster){
		CCSprite *asteroid = (CCSprite *)itAster;
		if ( ! asteroid->isVisible() )
			continue ;
		CCObject* itLaser;
		CCARRAY_FOREACH(_shipLasers, itLaser){
			CCSprite *shipLaser = (CCSprite *)itLaser;
			if ( ! shipLaser->isVisible() )
				continue ;
			if ( CCRect::CCRectIntersectsRect(shipLaser->boundingBox(), asteroid->boundingBox()) ) {
				shipLaser->setVisible(false) ;
				asteroid->setVisible(false) ;
				//music
				SimpleAudioEngine::sharedEngine()->playEffect("explosion_large.wav") ;
				continue ;
			}
		}  
		if ( CCRect::CCRectIntersectsRect(_ship->boundingBox(), asteroid->boundingBox()) ) {
			asteroid->setVisible(false) ;
			_ship->runAction( CCBlink::actionWithDuration(1.0, 9)) ;
			_lives-- ;
			//music
			SimpleAudioEngine::sharedEngine()->playEffect("explosion_large.wav") ;
		}
	}

	// §¿˚/ ß∞‹ºÏ≤‚
	if ( _lives <= 0 ) {
		_ship->stopAllActions() ;
		_ship->setVisible(false) ;
		this->endScene(KENDREASONLOSE) ;
	} else if ( curTimeMillis >= _gameOverTime ) {
		this->endScene(KENDREASONWIN) ;
	}
}

void HelloWorld::didAccelerate(CCAcceleration* pAccelerationValue) 
{
	#define KFILTERINGFACTOR 0.1
	#define KRESTACCELX -0.6
	#define KSHIPMAXPOINTSPERSEC (winSize.height*0.5)        
	#define KMAXDIFFX 0.2
 
	double rollingX ;
 
	// Cocos2DX inverts X and Y accelerometer depending on device orientation
	// in landscape mode right x=-y and y=x !!! (Strange and confusing choice)
	pAccelerationValue->x = pAccelerationValue->y ;
	rollingX = (pAccelerationValue->x * KFILTERINGFACTOR) + (rollingX * (1.0 - KFILTERINGFACTOR));    
	float accelX = pAccelerationValue->x - rollingX ;
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	float accelDiff = accelX - KRESTACCELX;
	float accelFraction = accelDiff / KMAXDIFFX;
	_shipPointsPerSecY = KSHIPMAXPOINTSPERSEC * accelFraction;   
}

float HelloWorld::randomValueBetween( float low , float high )
{
	//return (((float) arc4random() / 0xFFFFFFFFu) * (high - low)) + low;
	return CCRANDOM_0_1() * (high - low) + low;
}
 
float HelloWorld::getTimeTick() {
	timeval time;
	gettimeofday(&time, NULL);
	unsigned long millisecs = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	return (float) millisecs;
}

void HelloWorld::setInvisible(CCNode * node) {
	node->setVisible(false) ;
}

void HelloWorld::ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	//music
	SimpleAudioEngine::sharedEngine()->playEffect("laser_ship.wav") ;

    CCSize winSize = CCDirector::sharedDirector()->getWinSize() ; 
 
    CCSprite *shipLaser = (CCSprite *)_shipLasers->objectAtIndex(_nextShipLaser++);
    if ( _nextShipLaser >= _shipLasers->count() )
        _nextShipLaser = 0;
    shipLaser->setPosition( ccpAdd( _ship->getPosition(), ccp(shipLaser->getContentSize().width/2, 0)));
    shipLaser->setVisible(true) ;
    shipLaser->stopAllActions() ;
    shipLaser->runAction( CCSequence::actions (
        CCMoveBy::actionWithDuration(0.5,ccp(winSize.width, 0)),
        CCCallFuncN::actionWithTarget(this,callfuncN_selector(HelloWorld::setInvisible)) ,
        NULL  // DO NOT FORGET TO TERMINATE WITH NULL 
        ) ) ; 
}

void HelloWorld::restartTapped(CCObject* pSender) {
	CCDirector::sharedDirector()->replaceScene
		( CCTransitionZoomFlipX::transitionWithDuration(0.5, this->scene()));  
	// reschedule
	this->scheduleUpdate() ; 
}
 
void HelloWorld::endScene( EndReason endReason ) {
	if ( _gameOver )
	return ;
	_gameOver = true ;
 
	CCSize winSize = CCDirector::sharedDirector()->getWinSize() ; 
	char message[10] = "You Win"  ;
	if ( endReason == KENDREASONLOSE )
	strcpy(message,"You Lose") ;
	CCLabelBMFont * label ;
	label = CCLabelBMFont::labelWithString(message ,"Arial.fnt" );
	label->setScale(0.1) ;
	label->setPosition( ccp( winSize.width/2 , winSize.height*0.6)) ;
	this->addChild(label) ;  
 
	CCLabelBMFont * restartLabel ;
	restartLabel = CCLabelBMFont::labelWithString("Restart" ,"Arial.fnt" );
	CCMenuItemLabel *restartItem = CCMenuItemLabel::itemWithLabel(restartLabel, this, menu_selector(HelloWorld::restartTapped) );  
	restartItem->setScale(0.1) ;
	restartItem->setPosition( ccp( winSize.width/2 , winSize.height*0.4)) ;
 
	CCMenu *menu = CCMenu::menuWithItems(restartItem, NULL);
	menu->setPosition(CCPointZero);
	this->addChild(menu) ;
 
	// clear label and menu
	restartItem->runAction( CCScaleTo::actionWithDuration(0.5, 1.0)) ;
	label ->runAction( CCScaleTo::actionWithDuration(0.5, 1.0)) ;
	// Terminate update callback
	this->unscheduleUpdate() ;
}