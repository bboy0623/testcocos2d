//
//  HelloWorldLayer.m
//  cocos2d-2.x-ARC-iOS
//
//  Created by Steffen Itterheim on 27.04.12.
//  Copyright __MyCompanyName__ 2012. All rights reserved.
//

#include "TileMapLayer.h"

//#ifndef MAX
//#define MAX(x, y) ((x) > (y)) ? (x) : (y)
//#endif
//#ifndef MIN
//#define MIN(x, y) ((x) < (y)) ? (x) : (y)
//#endif

CCScene* TileMapLayer::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        TileMapLayer *layer = TileMapLayer::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool TileMapLayer::init()
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
            menu_selector(TileMapLayer::menuCloseCallback));
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
        //CCLabelTTF* pLabel = CCLabelTTF::create("Hello World", "Arial", 24);
        //CC_BREAK_IF(! pLabel);
        //CCSize size = CCDirector::sharedDirector()->getWinSize();
        //pLabel->setPosition(ccp(size.width / 2, size.height - 50));
        //this->addChild(pLabel, 1);
        //// 3. Add add a splash screen, show the cocos2d splash image.
        //CCSprite* pSprite = CCSprite::create("HelloWorld.png");
        //CC_BREAK_IF(! pSprite);
        //pSprite->setPosition(ccp(size.width/2, size.height/2));
        //this->addChild(pSprite, 0);

		CCTMXTiledMap* tileMap = CCTMXTiledMap::tiledMapWithTMXFile("tmx/isometric-with-border.tmx");
		this->addChild(tileMap, -1, TileMapNode);		
		
		CCTMXLayer* collisionsLayer = tileMap->layerNamed("Collisions");
		collisionsLayer->setVisible(false);

		// Use a negative offset to set the tilemap's start position
		tileMap->setPosition(CCPointMake(-500, -500));
		
		this->setTouchEnabled(true);
		
		const int borderSize = 10;
		playableAreaMin = CCPointMake(borderSize, borderSize);
		playableAreaMax = CCPointMake(tileMap->getMapSize().width - 1 - borderSize,
									  tileMap->getMapSize().height - 1 - borderSize);
		
		CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
		
		// Create the player and add it
		player = Player::create();
		player->setPosition(CCPointMake(screenSize.width / 2, screenSize.height / 2));
        // offset player's texture to best match the tile center position
		player->setAnchorPoint(CCPointMake(0.3f, 0.1f));
		this->addChild(player);
		
		// divide the screen into 4 areas
		screenCenter = CCPointMake(screenSize.width / 2, screenSize.height / 2);
		upperLeft = CCRectMake(0, screenCenter.y, screenCenter.x, screenCenter.y);
		lowerLeft = CCRectMake(0, 0, screenCenter.x, screenCenter.y);
		upperRight = CCRectMake(screenCenter.x, screenCenter.y, screenCenter.x, screenCenter.y);
		lowerRight = CCRectMake(screenCenter.x, 0, screenCenter.x, screenCenter.y);
		
		// to move in any of these directions means to add/subtract 1 to/from the current tile coordinate
		moveOffsets[MoveDirectionNone] = CCPointZero;
		moveOffsets[MoveDirectionUpperLeft] = CCPointMake(-1, 0);
		moveOffsets[MoveDirectionLowerLeft] = CCPointMake(0, 1);
		moveOffsets[MoveDirectionUpperRight] = CCPointMake(0, -1);
		moveOffsets[MoveDirectionLowerRight] = CCPointMake(1, 0);
		
		currentMoveDirection = MoveDirectionNone;
		
		// continuously check for walking
		this->scheduleUpdate();

        bRet = true;
    } while (0);

    return bRet;
}

void TileMapLayer::menuCloseCallback(CCObject* pSender)
{
    // "close" menu item clicked
    CCDirector::sharedDirector()->end();
}

bool isTilePosBlocked(CCPoint tilePos, CCTMXTiledMap* tileMap)
{
	CCTMXLayer* layer = tileMap->layerNamed("Collisions");
	//NSAssert(layer != nil, @"Collisions layer not found!");
	
	bool isBlocked = false;
	unsigned int tileGID = layer->tileGIDAt(tilePos);
	if (tileGID > 0)
	{
		CCDictionary* tileProperties = tileMap->propertiesForGID(tileGID);
		CCObject* blocks_movement = tileProperties->objectForKey("blocks_movement");		
		isBlocked = (blocks_movement != NULL);
	}
	
	return isBlocked;
}

CCPoint TileMapLayer::ensureTilePosIsWithinBounds(CCPoint tilePos)
{
	// make sure coordinates are within bounds of the playable area
	tilePos.x = MAX(playableAreaMin.x, tilePos.x);
	tilePos.x = MIN(playableAreaMax.x, tilePos.x);
	tilePos.y = MAX(playableAreaMin.y, tilePos.y);
	tilePos.y = MIN(playableAreaMax.y, tilePos.y);
	
	return tilePos;
}

CCPoint floatTilePosFromLocation(CCPoint location, CCTMXTiledMap* tileMap)
{
	// Tilemap position must be added as an offset, in case the tilemap position is not at 0,0 due to scrolling
	CCPoint pos = ccpSub(location, tileMap->getPosition());
	
	float halfMapWidth = tileMap->getMapSize().width * 0.5f;
	float mapHeight = tileMap->getMapSize().height;
	float tileWidth = tileMap->getTileSize().width / CC_CONTENT_SCALE_FACTOR();
	float tileHeight = tileMap->getTileSize().height / CC_CONTENT_SCALE_FACTOR();
	
	CCPoint tilePosDiv = CCPointMake(pos.x / tileWidth, pos.y / tileHeight);
	float mapHeightDiff = mapHeight - tilePosDiv.y;
	
	// Cast to int makes sure that result is in whole numbers, tile coordinates will be used as array indices
	float posX = (mapHeightDiff + tilePosDiv.x - halfMapWidth);
	float posY = (mapHeightDiff - tilePosDiv.x + halfMapWidth);
	
	return CCPointMake(posX, posY);
}

CCPoint TileMapLayer::tilePosFromLocation(CCPoint location, CCTMXTiledMap* tileMap)
{
	CCPoint pos = floatTilePosFromLocation(location, tileMap);
	
	// make sure coordinates are within bounds of the playable area, and cast to int
	pos = ensureTilePosIsWithinBounds(CCPointMake((int)pos.x, (int)pos.y));
	
	//CCLOG(@"touch at (%.0f, %.0f) is at tileCoord (%i, %i)", location.x, location.y, (int)pos.x, (int)pos.y);
	
	return pos;
}

void TileMapLayer::centerTileMapOnTileCoord(CCPoint tilePos, CCTMXTiledMap* tileMap)
{
	// get the ground layer
	CCTMXLayer* layer = tileMap->layerNamed("Ground");
	//NSAssert(layer != nil, @"Ground layer not found!");
	
	// internally tile Y coordinates are off by 1, this fixes the returned pixel coordinates
	tilePos.y -= 1;
	
	// get the pixel coordinates for a tile at these coordinates
	CCPoint scrollPosition = layer->positionAt(tilePos);
	// negate the position for scrolling
	scrollPosition = ccpMult(scrollPosition, -1);
	// add offset to screen center
	scrollPosition = ccpAdd(scrollPosition, screenCenter);
	
	CCLOG("tilePos: (%i, %i) moveTo: (%.0f, %.0f)", (int)tilePos.x, (int)tilePos.y, scrollPosition.x, scrollPosition.y);
	
	CCAction* move = CCMoveTo::actionWithDuration(0.2f, scrollPosition);
	tileMap->stopAllActions();
	tileMap->runAction(move);
}

CCPoint locationFromTouch(CCTouch* touch)
{
	//CCPoint touchLocation = [touch locationInView:touch.view];
	CCPoint touchLocation = touch->locationInView();
	return CCDirector::sharedDirector()->convertToGL(touchLocation);
}

CCPoint locationFromTouches(CCSet* touches)
{
	return locationFromTouch((CCTouch*)touches->anyObject());
}

void TileMapLayer::ccTouchesBegan(CCSet * touches, CCEvent * event)
{
	// get the position in tile coordinates from the touch location
	CCPoint touchLocation = locationFromTouches(touches);
	
	// check on which screen quadrant the touch was and set the move direction accordingly
	if (CCRect::CCRectContainsPoint(upperLeft, touchLocation))
	{
		currentMoveDirection = MoveDirectionUpperLeft;
	}
	else if (CCRect::CCRectContainsPoint(lowerLeft, touchLocation))
	{
		currentMoveDirection = MoveDirectionLowerLeft;
	}
	else if (CCRect::CCRectContainsPoint(upperRight, touchLocation))
	{
		currentMoveDirection = MoveDirectionUpperRight;
	}
	else if (CCRect::CCRectContainsPoint(lowerRight, touchLocation))
	{
		currentMoveDirection = MoveDirectionLowerRight;
	}
}

void TileMapLayer::ccTouchesEnded(CCSet * touches, CCEvent *event)
{
	currentMoveDirection = MoveDirectionNone;
}

void TileMapLayer::update(float delta)
{
	CCNode* node = this->getChildByTag(TileMapNode);
	//NSAssert([node isKindOfClass:[CCTMXTiledMap class]], @"not a CCTMXTiledMap");
	CCTMXTiledMap* tileMap = (CCTMXTiledMap*)node;
	
	// if the tilemap is currently being moved, wait until it's done moving
	if (tileMap->numberOfRunningActions() == 0)
	{
		if (currentMoveDirection != MoveDirectionNone)
		{
			// player is always standing on the tile which is centered on the screen
			CCPoint tilePos = tilePosFromLocation(screenCenter, tileMap);
			
			// get the tile coordinate offset for the direction we're moving to
			//NSAssert(currentMoveDirection < MAX_MoveDirections, @"invalid move direction!");
			CCPoint offset = moveOffsets[currentMoveDirection];
			
			// offset the tile position and then make sure it's within bounds of the playable area
			tilePos = CCPointMake(tilePos.x + offset.x, tilePos.y + offset.y);
			tilePos = ensureTilePosIsWithinBounds(tilePos);
			
			if (isTilePosBlocked(tilePos, tileMap) == false)
			{
				// move tilemap so that touched tiles is at center of screen
				centerTileMapOnTileCoord(tilePos, tileMap);
			}
		}
	}
	
	// continuously fix the player's Z position
	CCPoint tilePos = floatTilePosFromLocation(screenCenter, tileMap);
	player->updateVertexZ(tilePos, tileMap);
}


