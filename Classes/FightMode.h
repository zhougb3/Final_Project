#ifndef __FIGHTMODE_H__
#define __FIGHTMODE_H__

#include "cocos2d.h"
USING_NS_CC;

class FightMode : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	virtual bool onTouchBegan(Touch *touch, Event *unused_event);

	virtual void shootCallback(Ref* pSender);

	CREATE_FUNC(FightMode);

private:
	Sprite* player1;

	Sprite* player2;

	Layer* enermyLayer;

};

#endif // __FIGHTMODE_H__