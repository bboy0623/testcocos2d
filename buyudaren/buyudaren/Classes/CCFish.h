//
//  CCFish.h
//  fish
//
//  Created by  on 12-4-12.
//  Copyright 2012Äê __MyCompanyName__. All rights reserved.
//

#include "cocos2d.h"

using namespace cocos2d;

typedef enum 
{
    FishNoraml = 0,
    FishGold,
    FishShark
}FishType;


class CCFish : public CCSprite
{
public:
	CREATE_FUNC(CCFish);
	bool randomCatch(int Level);
	void addPath();
	void moveWithParabola(CCSprite* mSprite, CCPoint startPoint, CCPoint endPoint, float startAngle, float endAngle, float time);
	bool isCatch(){
		return this->m_Catch;
	}
	void setCatch(bool value){
		this->m_Catch = value;
	}
private:
    FishType type;
    bool m_Catch;
    CCAction *path;

	void removeSelf(CCNode *sender);
};


