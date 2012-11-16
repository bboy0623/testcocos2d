//
//  UiNumRoll.m
//  WheelScore
//
//  Created by 周海锋 on 12-4-8.
//  Copyright 2012年 CJLU. All rights reserved.
//

#include "UINumber.h"
//@implementation UINumber
//@synthesize m_texture;

/*
 * init 初始化
 */
//-(id) init
bool UINumber::init()
{
	//if( (self=[super init])) {
        m_texture = NULL;
        m_style = NumStyleNormal;
        m_num = 0;
        m_nPosCur = 0;
        m_nPosEnd = 0;
        //[self setup];
		this->setup();
    //}
	//return self;
	return true;
}

void UINumber::init(NumStyle style)
{
    m_texture = NULL;
    m_style = style;
    m_num = 0;
    m_nPosCur = 0;
    m_nPosEnd = 0;
}

/*
 * initWithStyle 初始化
 */
UINumber* UINumber::initWithStyle(NumStyle style)
{
    //if( (self=[super init])) {
	UINumber* self = UINumber::create();
	self->init(style);
    self->autorelease();   
    //}
    return self;
}

/*
 * setup 设置texture
 */
void UINumber::setup()
{
	//UIImage *image = [UIImage imageNamed:@"number.png"];
	CCImage *image = new CCImage();
	image->initWithImageFile("number.png");
	image->autorelease();

    //m_texture = [[CCTexture2D alloc]initWithImage:image];
	m_texture = new CCTexture2D();
	m_texture->autorelease();
	m_texture->initWithImage(image);

	//CCSpriteFrame *frame = [CCSpriteFrame frameWithTexture:m_texture rect:CGRectMake(0, 0, NUM_WIDTH, NUM_HEIGHT)];
    CCSpriteFrame *frame = CCSpriteFrame::createWithTexture(m_texture, CCRectMake(0, 0, NUM_WIDTH, NUM_HEIGHT));
	this->setDisplayFrame(frame);
}

/*
 * setNumber 设置显示的数字
 */
void UINumber::setNumber(int num)
{
    m_nPosCur = NUM_HEIGHT * m_num;
    m_nPosEnd = NUM_HEIGHT * num;
    if (NumStyleNormal == m_style) {
        m_nMoveLen = 4;
    }
    else if (NumStyleSameTime == m_style) {
        m_nMoveLen = (m_nPosEnd-m_nPosCur)/20;
    }
    
    if (m_num > num) {		
        //[self schedule:@selector(onRollUP:) interval:0.03];
		this->schedule(schedule_selector(UINumber::onRollUP), 0.03);
    }
    else {
        //[self schedule:@selector(onRollDown:) interval:0.03];
		this->schedule(schedule_selector(UINumber::onRollDown), 0.03);
    }
    m_num = num;
}

/*
 * onRollDown 向下滚动
 */
void UINumber::onRollDown(float dt)
{
    m_nPosCur += m_nMoveLen;
    if (m_nPosCur >= m_nPosEnd) {
        m_nPosCur = m_nPosEnd;
        //[self unschedule:@selector(onRollDown:)];
		this->unschedule(schedule_selector(UINumber::onRollDown));
    }
    
    //CCSpriteFrame *frame = [CCSpriteFrame frameWithTexture:m_texture rect:CGRectMake(0, m_nPosCur, NUM_WIDTH, NUM_HEIGHT)];
	CCSpriteFrame *frame = CCSpriteFrame::createWithTexture(m_texture, CCRectMake(0, m_nPosCur, NUM_WIDTH, NUM_HEIGHT));    
	this->setDisplayFrame(frame);
}


/*
 * onRollUP 向上滚动
 */
void UINumber::onRollUP(float dt)
{
    m_nPosCur -= 4;
    if (m_nPosCur <= m_nPosEnd) {
        m_nPosCur = m_nPosEnd;
        //[self unschedule:@selector(onRollUP:)];
		this->unschedule(schedule_selector(UINumber::onRollUP));
    }
    
    //CCSpriteFrame *frame = [CCSpriteFrame frameWithTexture:m_texture rect:CGRectMake(0, m_nPosCur, NUM_WIDTH, NUM_HEIGHT)];
	CCSpriteFrame *frame = CCSpriteFrame::createWithTexture(m_texture, CCRectMake(0, m_nPosCur, NUM_WIDTH, NUM_HEIGHT));
    this->setDisplayFrame(frame);
}

//void dealloc
//{
//    [self unscheduleAllSelectors];
//    [m_texture release];
//    [super dealloc];
//}

