//
//  UIRollNum.h
//  WheelScore
//
//  Created by �ܺ��� on 12-4-8.
//  Copyright 2012�� CJLU. All rights reserved.
//

#include "cocos2d.h"
#include "UINumber.h"

using namespace cocos2d;

class UIRollNum : public CCSprite {
public:
	void rebuildEffect();
	void clearEffect();
	int getNumber();
	void setNumber(int num);

	virtual bool init();
	CREATE_FUNC(UIRollNum);
private:
    int m_nNumber;              //��ʾ������
    int m_maxCol;               //�����ʾλ��
    CCArray *numArray;   //���ÿ�����ֵ�����
    CCPoint m_point;            //����
    bool  zeroFill;             //�Ƿ���0���
    NumStyle style;             //������ʽ
};

//@property (nonatomic,retain) NSMutableArray *numArray;
//@property (nonatomic) CGPoint m_point;
//@property (nonatomic) NumStyle style;  



