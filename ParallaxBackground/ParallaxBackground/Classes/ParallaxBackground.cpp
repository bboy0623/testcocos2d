//#include <stdio.h>
#include <string>
#include "ParallaxBackground.h"

bool ParallaxBackground::init()
{
	cocos2d::CCSize screenSize = cocos2d::CCDirector::sharedDirector()->getWinSize();

	//CCSpriteFrameCache* frameCache = [CCSpriteFrameCache sharedSpriteFrameCache];
	//[frameCache addSpriteFramesWithFile:@"game-art.plist"];
	cocos2d::CCSpriteFrameCache* frameCache = cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache();
	frameCache->addSpriteFramesWithFile("game-art.plist");

	//CCTexture2D* gameArtTexture = [[CCTextureCache sharedTextureCache] addImage:@"game-art.png"];
	cocos2d::CCTexture2D* gameArtTexture = cocos2d::CCTextureCache::sharedTextureCache()->addImage("game-art.png");

	this->spriteBatch = cocos2d::CCSpriteBatchNode::batchNodeWithTexture(gameArtTexture);
	this->addChild(this->spriteBatch);

	numStripes = 7;

	for (int i = 0; i < numStripes; i++){
		//NSString* frameName = [NSString stringWithFormat:@"bg%i.png", i];
		char frameName[50];
		memset(frameName, 0, 50);
		sprintf(frameName, "bg%i.png", i);

		cocos2d::CCSprite* sprite = cocos2d::CCSprite::spriteWithSpriteFrameName(frameName);
		sprite->setAnchorPoint(cocos2d::CCPointMake(0, 0.5f));
		sprite->setPosition(cocos2d::CCPointMake(0, screenSize.height / 2));
		spriteBatch->addChild(sprite, i, i);
	}

	// Add 7 more stripes, flip them and position them next to their neighbor stripe
	for (int i = 0; i < numStripes; i++){
		char frameName[50];
		memset(frameName, 0, 50);
		sprintf(frameName, "bg%i.png", i);			

		cocos2d::CCSprite *sprite = cocos2d::CCSprite::spriteWithSpriteFrameName(frameName);				
		sprite->setAnchorPoint(cocos2d::CCPointMake(0, 0.5f));
		sprite->setPosition(cocos2d::CCPointMake(screenSize.width - 1, screenSize.height / 2));

		// Flip the sprite so that it aligns perfectly with its neighbor
		sprite->setFlipX(true);			
		// Add the sprite using the same tag offset by numStripes
		//[spriteBatch addChild:sprite z:i tag:i + numStripes];
		spriteBatch->addChild(sprite, i, i);
	}

	this->speedFactors[0] = 0.3f;
	this->speedFactors[1] = 0.5f;
	this->speedFactors[2] = 0.5f;
	this->speedFactors[3] = 0.8f;
	this->speedFactors[4] = 0.8f;
	this->speedFactors[5] = 1.2f;
	this->speedFactors[6] = 1.2f;

	scrollSpeed = 1.0f;
	//[self scheduleUpdate];
	this->scheduleUpdate();

	return TRUE;
}

void ParallaxBackground::update (float dt)
{
	cocos2d::CCObject* item;

	CCARRAY_FOREACH(spriteBatch->getChildren(), item){

		cocos2d::CCSprite* sprite = (cocos2d::CCSprite*)item;
		//CCLOG(@"tag: %i", sprite.tag);
		float factor = speedFactors[sprite->getZOrder()];

		cocos2d::CCPoint pos = sprite->getPosition();
		pos.x -= scrollSpeed * factor;

		if (pos.x < -screenSize.width){
			pos.x += (screenSize.width * 2) - 2;
		}

		sprite->setPosition(pos);	
	}
}