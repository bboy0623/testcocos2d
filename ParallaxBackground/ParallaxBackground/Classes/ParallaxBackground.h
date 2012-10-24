#include "cocos2d.h"

class ParallaxBackground : public cocos2d::CCNode
{
public:
    virtual bool init();      

	virtual void update (float dt);
    // implement the "static node()" method manually
    CREATE_FUNC(ParallaxBackground);
private:
	cocos2d::CCSpriteBatchNode* spriteBatch;

	int numStripes;

	//cocos2d::CCArray* speedFactors;
	float speedFactors[7];
	float scrollSpeed;

	cocos2d::CCSize screenSize;
	//cocos2d::CGSize screenSize;
};
