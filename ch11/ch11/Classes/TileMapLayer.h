#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#ifdef WIN32
#include "Box2D/Box2D.h"
#endif
#include "SimpleAudioEngine.h"

#include "Player.h"

using namespace cocos2d;

enum
{
	TileMapNode = 0,
};

typedef enum
{
	MoveDirectionNone = 0,
	MoveDirectionUpperLeft,
	MoveDirectionLowerLeft,
	MoveDirectionUpperRight,
	MoveDirectionLowerRight,
	
	MAX_MoveDirections,
} EMoveDirection;

class TileMapLayer : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);

    // implement the "static node()" method manually
    CREATE_FUNC(TileMapLayer);

private:
	CCPoint playableAreaMin, playableAreaMax;
	Player* player;

	CCPoint screenCenter;
	CCRect upperLeft, lowerLeft, upperRight, lowerRight;
	CCPoint moveOffsets[MAX_MoveDirections];
	EMoveDirection currentMoveDirection;

	CCPoint ensureTilePosIsWithinBounds(CCPoint tilePos);
	CCPoint tilePosFromLocation(CCPoint location, CCTMXTiledMap* tileMap);
	void centerTileMapOnTileCoord(CCPoint tilePos, CCTMXTiledMap* tileMap);

	void ccTouchesBegan(CCSet * touches, CCEvent * event);
	void ccTouchesEnded(CCSet * touches, CCEvent *event);
	void update(float delta);
};

#endif  // __HELLOWORLD_SCENE_H__