#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "cocos2d.h"

using namespace cocos2d;

class Player : public cocos2d::CCSprite
{
public:
	virtual bool init();  

	CREATE_FUNC(Player);

	void updateVertexZ(CCPoint tilePos, CCTMXTiledMap* tileMap);
};

#endif