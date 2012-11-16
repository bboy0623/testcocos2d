//
//  UIRollNum.h
//  WheelScore
//
//  Created by 周海锋 on 12-4-8.
//  Copyright 2012年 CJLU. All rights reserved.
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
    int m_nNumber;              //显示的数字
    int m_maxCol;               //最大显示位数
    CCArray *numArray;   //存放每个数字的数组
    CCPoint m_point;            //坐标
    bool  zeroFill;             //是否开启0填充
    NumStyle style;             //滚动样式
};

//@property (nonatomic,retain) NSMutableArray *numArray;
//@property (nonatomic) CGPoint m_point;
//@property (nonatomic) NumStyle style;  



