#include "HelloWorldScene.h"

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
		
		CCSize size = CCDirector::sharedDirector()->getWinSize();				

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
        //pLabel->setPosition(ccp(size.width / 2, size.height - 50));
        //// Add the label to HelloWorld layer as a child layer.
        //this->addChild(pLabel, 1);

        // 3. Add add a splash screen, show the cocos2d splash image.
  //      pSprite = CCSprite::create("HelloWorld.png");
  //      CC_BREAK_IF(! pSprite);
  //      pSprite->setPosition(ccp(size.width/3, size.height/3));
		//pSprite->setAnchorPoint(ccp(0, 0));
  //      this->addChild(pSprite, 0);

		CCTMXTiledMap* tileMap = CCTMXTiledMap::tiledMapWithTMXFile("tmx/orthogonal.tmx");
		this->addChild(tileMap, -1, TileMapNode);
		CCTMXLayer* eventLayer = tileMap->layerNamed("GameEventLayer");
		eventLayer->setVisible(false);
		
		//tileMap->layerNamed("WinterLayer")->setVisible(false);
		//tileMap->layerNamed("Background")->setVisible(false);		

		this->setTouchEnabled(true);

        bRet = true;
    } while (0);

    return bRet;
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    // "close" menu item clicked
    CCDirector::sharedDirector()->end();
}

void HelloWorld::ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
	CCNode* node = this->getChildByTag(TileMapNode);	
	//NSAssert([node isKindOfClass:[CCTMXTiledMap class]], @"not a CCTMXTiledMap");
	CCTMXTiledMap* tileMap = (CCTMXTiledMap*)node;
	
	// get the position in tile coordinates from the touch location
	CCPoint touchLocation = this->locationFromTouch((cocos2d::CCTouch *)pTouches->anyObject());	
	CCPoint tilePos = this->tilePosFromLocation(touchLocation, tileMap);	

	//// move tilemap so that touched tiles is at center of screen
	this->centerTileMapOnTileCoord(tilePos, tileMap);

	// Check if the touch was on water (eg. tiles with isWater property drawn in GameEventLayer)
	bool isTouchOnWater = false;	
	CCTMXLayer* eventLayer = tileMap->layerNamed("GameEventLayer");
	int tileGID = eventLayer->tileGIDAt(tilePos);	
	//
	if (tileGID != 0) {
		CCDictionary* properties = tileMap->propertiesForGID(tileGID);
		if (properties) {
			//CCLOG(@"NSDictionary 'properties' contains:\n%@", properties);	
			const CCString * isWaterProperty = properties->valueForKey("isWater");
			isTouchOnWater = isWaterProperty->boolValue();			
		}
	}
	
	// Check if the touch was within one of the rectangle objects
	CCTMXObjectGroup* objectLayer = tileMap->objectGroupNamed("ObjectLayer");
	//NSAssert([objectLayer isKindOfClass:[CCTMXObjectGroup class]], 
	//		 @"ObjectLayer not found or not a CCTMXObjectGroup");
	
	bool isTouchInRectangle = false;
	int numObjects = objectLayer->getObjects()->count();
	for (int i = 0; i < numObjects; i++){
		CCDictionary* properties = (CCDictionary*)objectLayer->getObjects()->objectAtIndex(i);
		CCRect rect = this->getRectFromObjectProperties(properties, tileMap);
		//	
		if (CCRect::CCRectContainsPoint(rect, touchLocation)) {
			isTouchInRectangle = true;
			break;
		}
	}
	
	// decide what to do depending on where the touch was ...
	if (isTouchOnWater) {
		//[[SimpleAudioEngine sharedEngine] playEffect:@"alien-sfx.caf"];
		CCLog("touchOnWater");
	}
	else if (isTouchInRectangle) {
		CCLog("touchObject");
		CCParticleSystem* system = CCParticleSystemQuad::particleWithFile("fx-explosion.plist");
		system->setAutoRemoveOnFinish(true);
		system->setPosition(touchLocation);
		this->addChild(system, 1);
	}
	else {

#if 0
		// get the winter layer and toggle its visibility
		CCTMXLayer* winterLayer = tileMap->layerNamed("WinterLayer");
		winterLayer->setVisible(!winterLayer->isVisible());
		
		// other options you might be interested in are:	

		// remove the touched tile		
		winterLayer->removeTileAt(tilePos);		
		// add a specific tile
		tileGID = winterLayer->tileGIDAt(CCPointMake(0, 19));
		winterLayer->setTileGID(tileGID, tilePos);
#endif

	}
}

CCPoint HelloWorld::locationFromTouch(CCTouch* touch)
{
	CCPoint touchLocation = touch->locationInView();		
	return CCDirector::sharedDirector()->convertToGL(touchLocation);
}

//-(void) centerTileMapOnTileCoord:(CGPoint)tilePos tileMap:(CCTMXTiledMap*)tileMap
void HelloWorld::centerTileMapOnTileCoord(cocos2d::CCPoint tilePos, CCTMXTiledMap* tileMap)
{
	// center tilemap on the given tile pos
	CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
	CCPoint screenCenter = CCPointMake(screenSize.width * 0.5f, screenSize.height * 0.5f);
	
	// tile coordinates are counted from upper left corner, this maps coordinates to lower left corner
	tilePos.y = (tileMap->getMapSize().height - 1) - tilePos.y;

	// scaling tileSize to Retina display size
	float pointWidth = tileMap->getTileSize().width / CC_CONTENT_SCALE_FACTOR();
	float pointHeight = tileMap->getTileSize().height / CC_CONTENT_SCALE_FACTOR();

	// point is now at lower left corner of the screen
	CCPoint scrollPosition = CCPointMake(-(tilePos.x * pointWidth), -(tilePos.y * pointHeight));
	
	// offset point to center of screen and center of tile
	scrollPosition.x += screenCenter.x - pointWidth * 0.5f;
	scrollPosition.y += screenCenter.y - pointHeight * 0.5f;
	
	// make sure tilemap scrolling stops at the tilemap borders
	scrollPosition.x = MIN(scrollPosition.x, 0);
	scrollPosition.x = MAX(scrollPosition.x, -screenSize.width);
	scrollPosition.y = MIN(scrollPosition.y, 0);
	scrollPosition.y = MAX(scrollPosition.y, -screenSize.height);
	
	CCLOG("tilePos: (%i, %i) moveTo: (%.0f, %.0f)", 
		  (int)tilePos.x, (int)tilePos.y, scrollPosition.x, scrollPosition.y);
	
	
	CCAction* move = CCMoveTo::actionWithDuration(0.2f, scrollPosition);	
	tileMap->stopAllActions();	
	tileMap->runAction(move);
}

CCRect HelloWorld::getRectFromObjectProperties(CCDictionary* dict, CCTMXTiledMap* tileMap)
{
	float x, y, width, height;
	x = dict->valueForKey("x")->floatValue() + tileMap->getPosition().x;
	y = dict->valueForKey("y")->floatValue() + tileMap->getPosition().y;
	width = dict->valueForKey("width")->floatValue();
	height = dict->valueForKey("height")->floatValue();
	
	return CCRectMake(x, y, width, height);
}

cocos2d::CCPoint HelloWorld::tilePosFromLocation(cocos2d::CCPoint location, cocos2d::CCTMXTiledMap* tileMap)
{
	// Tilemap position must be subtracted, in case the tilemap position is not at 0,0 due to scrolling
	CCPoint pos = ccpSub(location, tileMap->getPosition());
	
	// scaling tileSize to Retina display size
	float pointWidth = tileMap->getTileSize().width / CC_CONTENT_SCALE_FACTOR();
	float pointHeight = tileMap->getTileSize().height / CC_CONTENT_SCALE_FACTOR();
	
	// Cast to int makes sure that result is in whole numbers, tile coordinates will be used as array indices
	pos.x = (int)(pos.x / pointWidth);
	pos.y = (int)((tileMap->getMapSize().height * pointHeight - pos.y) / pointHeight);
	
	CCLOG("touch at (%.0f, %.0f) is at tileCoord (%i, %i)", location.x, location.y, (int)pos.x, (int)pos.y);
	if (pos.x < 0 || pos.y < 0 || pos.x >= tileMap->getMapSize().width || pos.y >= tileMap->getMapSize().height)
	{
		//CCLOG("%@: coordinates (%i, %i) out of bounds! Adjusting...", NSStringFromSelector(_cmd), (int)pos.x, (int)pos.y);		
	}

	pos.x = max(0, min(tileMap->getMapSize().width - 1, pos.x));
	pos.y = max(0, min(tileMap->getMapSize().height - 1, pos.y));

	return pos;
}


//#ifdef DEBUG
//// Draw the object rectangles for debugging and illustration purposes.
//-(void) draw
//{	
//	[super draw];
//	
//	CCNode* node = [self getChildByTag:TileMapNode];
//	NSAssert([node isKindOfClass:[CCTMXTiledMap class]], @"not a CCTMXTiledMap");
//	CCTMXTiledMap* tileMap = (CCTMXTiledMap*)node;
//	
//	// get the object layer
//	CCTMXObjectGroup* objectLayer = [tileMap objectGroupNamed:@"ObjectLayer"];
//	NSAssert([objectLayer isKindOfClass:[CCTMXObjectGroup class]], 
//			 @"ObjectLayer not found or not a CCTMXObjectGroup");
//	
//	// make the lines thicker
//	glLineWidth(2.0f * CC_CONTENT_SCALE_FACTOR());
//	ccDrawColor4F(1, 0, 1, 1);
//	
//	int numObjects = objectLayer.objects.count;
//	for (int i = 0; i < numObjects; i++)
//	{
//		NSDictionary* properties = [objectLayer.objects objectAtIndex:i];
//		CGRect rect = [self getRectFromObjectProperties:properties tileMap:tileMap];
//		
//		CGPoint dest = CGPointMake(rect.origin.x + rect.size.width, rect.origin.y + rect.size.height);
//		ccDrawRect(rect.origin, dest);
//		ccDrawSolidRect(rect.origin, dest, ccc4f(1, 0, 1, 0.3f));
//	}
//	
//	// reset line width & color as to not interfere with draw code in other nodes that draws lines
//	glLineWidth(1.0f);
//}
//#endif