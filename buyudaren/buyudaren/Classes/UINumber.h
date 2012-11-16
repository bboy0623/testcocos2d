//
//  UiNumRoll.h
//  WheelScore
//
//  Created by �ܺ��� on 12-4-8.
//  Copyright 2012�� CJLU. All rights reserved.
//

#include "cocos2d.h"

using namespace cocos2d;

#define NUM_HEIGHT 20
#define NUM_WIDTH  20

typedef enum{
    NumStyleNormal,
    NumStyleSameTime,
}NumStyle;

class UINumber : public CCSprite 
{
public:
	virtual bool init();
	CREATE_FUNC(UINumber);
	//@property(nonatomic,retain) CCTexture2D *m_texture;
	static UINumber* initWithStyle(NumStyle style);
	void setNumber(int num);
	void onRollDown(float dt);
	void onRollUP(float dt);
	void setup();

private:
	void UINumber::init(NumStyle style);

    NumStyle m_style;       //������ʽ
    int m_num;              //��ʾ������
    int m_nPosCur;          //��ǰ��λ��
    int m_nPosEnd;          //������λ��
    int m_nMoveLen;         //ÿ���ƶ���λ��
    CCTexture2D *m_texture; //���ֵ�texture
};


