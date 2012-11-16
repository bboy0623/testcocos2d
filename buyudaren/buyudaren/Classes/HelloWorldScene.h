#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

#include "Box2D/Box2D.h"

#include "SimpleAudioEngine.h"

#include  "CCFish.h"
#include  "CCNet.h"
#include  "UIRollNum.h"

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
    CCSpriteBatchNode *netSheet;
    CCSpriteBatchNode *fishSheet;
    CCSpriteBatchNode *fish2Sheet;
    CCSpriteBatchNode *cannonSheet;
    CCAction *fish01_act;
    UIRollNum *score1;
    CCSprite *gun;
    int Energy;
    int maxEnergy;
    CCSprite *engryPointer;

public:
	void LoadTexture();
	void initUITab();
	void addFish();
	void updateGame(float delta);
	void ShowNet(CCNode * sender);

	void updateEnergry(int en);
	void ccTouchesBegan(CCSet *touches, CCEvent *event);
	void ccTouchesEnded(CCSet *touches, CCEvent *event);
	void ShowNet(CCObject* sender);
	void afterShowNet(CCObject *sender);
	void afterShow(CCObject *sender);
	void afterCatch(CCSprite* sender);
};

#endif  // __HELLOWORLD_SCENE_H__