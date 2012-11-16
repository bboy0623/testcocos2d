//
//  CCFish.m
//  fish
//
//  Created by  on 12-4-12.
//  Copyright 2012�� __MyCompanyName__. All rights reserved.
//

#include "CCFish.h"



//@synthesize isCatch;

bool CCFish::randomCatch(int Level)
{
    if (rand()%10>=Level) {
        m_Catch = true;
    }else{
        m_Catch = false;
    }
    return m_Catch;
}

void CCFish::addPath()
{
    
    switch(rand()%7)
    {
        case 0:
            this->moveWithParabola(this, ccp(1200, 200), ccp(-500, 800), 0, 20, rand()%10+15);
            break;
        case 1:
            this->moveWithParabola(this, ccp(-200, 300), ccp(1300, 400), 180, 170, rand()%10+18);
            break;
        case 2:
            this->moveWithParabola(this, ccp(-200, 300), ccp(1000, -200), 190, 200, rand()%10+18);
            break;
        case 3:
            this->moveWithParabola(this, ccp(1300, 400), ccp(-200, 300), 10, 5, rand()%10+18);
            break;
        case 4:
            this->moveWithParabola(this, ccp(400, -1200), ccp(600, 1000), 90, 93, rand()%10+18);
            break;
            
        case 5:
            this->moveWithParabola(this, ccp(600, 1000), ccp(400, -200), -70, -80, rand()%10+18);
            break;
        case 6:
            this->moveWithParabola(this, ccp(1200, 2100), ccp(-200, 300), 30, -30, rand()%10+18);
            break;
    }
    
};


//void CCFish::moveWithParabola:(CCSprite*)mSprite startP:(CCPoint)startPoint endP:(CCPoint)endPoint startA:(float)startAngle endA:(float)endAngle dirTime:(float)time
void CCFish::moveWithParabola(CCSprite* mSprite, CCPoint startPoint, CCPoint endPoint, float startAngle, float endAngle, float time)
{ 

    float sx = startPoint.x;
    float sy = startPoint.y; 
    float ex =endPoint.x+rand()%50;
    float ey =endPoint.y+rand()%150; 
	int h = mSprite->getContentSize().height*0.5;
    //���þ������ʼ�Ƕ�
    CCPoint pos = CCPointMake(sx+-200+rand()%400, sy+-200+rand()%400);
	mSprite->setPosition(pos);
	mSprite->setRotation(startAngle);
    ccBezierConfig bezier; // ��������������
    bezier.controlPoint_1 = ccp(sx, sy); // ��ʼ��
    bezier.controlPoint_2 = ccp(sx+(ex-sx)*0.5, sy+(ey-sy)*0.5+rand()%300); //���Ƶ�
    bezier.endPosition = ccp(endPoint.x-30, endPoint.y+h); // ����λ��   
	CCBezierTo *actionMove = CCBezierTo::actionWithDuration(time, bezier);
    //����������ת�Ķ���
	CCRotateTo *actionRotate = CCRotateTo::actionWithDuration(time, endAngle);
    //������������װ��һ��ͬʱ���Ž��еĶ���
	//CCActionInterval * action = [CCSpawn actions:actionMove, actionRotate, nil]; 
	CCFiniteTimeAction * action = CCSpawn::create(actionMove, actionRotate, NULL); 
	//CCSequence *sq = [CCSequence actions:action,[CCCallFunc actionWithTarget:self selector:@selector(removeSelf:)],nil];
	CCFiniteTimeAction *sq = CCSequence::create(
			action, 
			CCCallFuncN::actionWithTarget(this, callfuncN_selector(CCFish::removeSelf)), 
			NULL
	);    
	mSprite->runAction(sq);
}

void CCFish::removeSelf(CCNode *sender)
{
    this->removeFromParentAndCleanup(true);
}

