#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

/**
 * cocos2d-2.0-x-2.0.2
 * http://www.raywenderlich.com/zh-hans/20010/cocos2d-x%E5%88%B6%E4%BD%9C%E5%A4%AA%E7%A9%BA%E5%B0%84%E5%87%BB%E6%B8%B8%E6%88%8F%E6%95%99%E7%A8%8B
**/

#include "cocos2d.h"

#include "Box2D/Box2D.h"

#include "SimpleAudioEngine.h"

#include "CCParallaxNodeExtras.h"

USING_NS_CC;

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

private:
	cocos2d::CCSpriteBatchNode * _batchNode;
	cocos2d::CCSprite * _ship;

	CCParallaxNodeExtras *_backgroundNode;  
	CCSprite *_spacedust1;
	CCSprite *_spacedust2;
	CCSprite *_planetsunrise;
	CCSprite *_galaxy;
	CCSprite *_spacialanomaly;
	CCSprite *_spacialanomaly2;

	virtual void update (float dt);

	//accelerate
	float _shipPointsPerSecY;
	virtual void didAccelerate(CCAcceleration* pAccelerationValue);

	//‘… Ø
	CCArray*  _asteroids ; 
	int _nextAsteroid ;
	float _nextAsteroidSpawn ;
public:
	//‘… Ø
	float randomValueBetween( float low , float high );
	void setInvisible(CCNode * node);
	float getTimeTick();

	//º§π‚
private:
	CCArray*  _shipLasers ; 
	int _nextShipLaser ;
public:
	virtual void ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event);

	//≈ˆ◊≤ºÏ≤‚
private:
	int _lives ;

	// §¿˚/ ß∞‹ºÏ≤‚

};

#endif  // __HELLOWORLD_SCENE_H__