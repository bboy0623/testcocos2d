#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
//#include "SimpleAudioEngine.h"

#ifdef WIN32
#include "Box2D/Box2D.h"
#else
#ifndef max
#define max(x, y) ((x) > (y)) ? (x) : (y)
#endif
#ifndef min
#define min(x, y) ((x) < (y)) ? (x) : (y)
#endif
#endif







using namespace cocos2d;

enum {
	TileMapNode = 0,
};
class HelloWorld : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);

    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);

	
	virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);

private:
	cocos2d::CCSprite* pSprite;

	CCPoint locationFromTouch(CCTouch* touch);
	void centerTileMapOnTileCoord(cocos2d::CCPoint tilePos, CCTMXTiledMap* tileMap);
	cocos2d::CCPoint tilePosFromLocation(cocos2d::CCPoint location, cocos2d::CCTMXTiledMap* tileMap);
	cocos2d::CCRect getRectFromObjectProperties(cocos2d::CCDictionary* dict, cocos2d::CCTMXTiledMap* tileMap);
};

#endif  // __HELLOWORLD_SCENE_H__