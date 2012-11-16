//
//  CCNet.h
//  fish
//
//  Created by  on 12-4-12.
//  Copyright 2012Äê __MyCompanyName__. All rights reserved.
//

#include "cocos2d.h"

using namespace cocos2d;

class CCNet : public CCSprite
{
public:
	CREATE_FUNC(CCNet);
	bool isCatching(){
		return this->m_Catching;
	}
	void setCatching(bool value){
		this->m_Catching = value;
	}
private:
    bool m_Catching;
};
