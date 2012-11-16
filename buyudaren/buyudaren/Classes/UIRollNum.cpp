//
//  UIRollNum.m
//  WheelScore
//
//  Created by 周海锋 on 12-4-8.
//  Copyright 2012年 CJLU. All rights reserved.
//

#include "UIRollNum.h"
//@implementation UIRollNum
//@synthesize numArray,m_point,style;

/*
 * init 初始化
 */
//-(id) init
bool UIRollNum::init()
{
    //if (self = [super init]) {
        m_nNumber = 0;
        m_maxCol = 6;
        //numArray =[[NSMutableArray alloc] init];
		numArray = new CCArray();
        zeroFill = true;
        style = NumStyleNormal;
        //[self clearEffect];
		this->clearEffect();
    //}   
    //return self;
	return true;
}

/*
 * getNumber 获取显示的数字
 */
int UIRollNum::getNumber()
{
    return m_nNumber;
}

/*
 * setNumber 设置显示的数字
 * num int 设置的数字
 */
void UIRollNum::setNumber(int num)
{
    if (m_nNumber != num) {
        m_nNumber = num;
       //[self rebuildEffect];
		this->rebuildEffect();
    }
}

/*
 * rebuildEffect 重新设置每位数字
 */
void UIRollNum::rebuildEffect()
{
        
    int i=0;
    int num = m_nNumber;
    while (1) {
        if (num<=0) {
            if(m_maxCol<=i && zeroFill)
            break;
        }
        int showNum = num%10;
        
        //UINumber* pNumber = [numArray objectAtIndex:i];
		UINumber* pNumber = (UINumber*)numArray->objectAtIndex(i);
        //[pNumber setNumber:showNum];
		pNumber->setNumber(showNum);
        i++;
        num = num/10;
    }
}

/*
 * rebuildEffect 清楚每位数字
 */
void UIRollNum::clearEffect()
{
	for(int i=0;i<numArray->count();i++) {
        
        //UINumber* pNumber = (UINumber *)[numArray objectAtIndex:i];
		UINumber* pNumber = (UINumber*)numArray->objectAtIndex(i);
        //[self removeChild:pNumber cleanup:YES];
		this->removeChild(pNumber, true);
    }
    //[numArray removeAllObjects];
	numArray->removeAllObjects();
    
    for (int i=0; i< m_maxCol; i++) {
        //UINumber* pNumber = [[UINumber alloc]initWithStyle:style];
		UINumber* pNumber = UINumber::initWithStyle(style);        
		numArray->addObject(pNumber);        
		pNumber->setNumber(0);        
		pNumber->setPosition(CCPointMake(m_point.x - i*NUM_WIDTH, m_point.y));
        pNumber->setAnchorPoint(CCPointMake(1, 0.5));
		this->addChild(pNumber, 100);

    }
}

//void dealloc
//{
//    [numArray release];
//     [super dealloc];
//}


