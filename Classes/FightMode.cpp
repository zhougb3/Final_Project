#include "FightMode.h"

USING_NS_CC;

Scene* FightMode::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = FightMode::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool FightMode::init()
{

	if (!Layer::init())
	{
		return false;
	}
	//变量初始化
	x_num = 35;
	y_num = 30;
	player1ADIsMove = false;
	player1WSIsMove = false;
	lastkey1 = 'D';
	player2ADIsMove = false;
	player2WSIsMove = false;
	lastkey2 = 'A';

	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	auto bg = Sprite::create("FightBackground.JPG");
	bg->setPosition(origin.x, origin.y);
	bg->setAnchorPoint(Vec2(0, 0));
	float bgx = bg->getTextureRect().getMaxX();
	float bgy = bg->getTextureRect().getMaxY();
	bg->setScaleX(visibleSize.height / bgy);
	bg->setScaleY(visibleSize.height / bgy);
	this->addChild(bg, 0);

	//hp条
	Sprite* HPMP1 = Sprite::create("HPMP.png", CC_RECT_PIXELS_TO_POINTS(Rect(7, 14, 1414, 275)));
	Sprite* HP1 = Sprite::create("HPMP.png", CC_RECT_PIXELS_TO_POINTS(Rect(1480, 57, 24, 41)));
	Sprite* MP1 = Sprite::create("HPMP.png", CC_RECT_PIXELS_TO_POINTS(Rect(1480, 184, 24, 41)));
	Sprite* HPMP2 = Sprite::create("HPMP.png", CC_RECT_PIXELS_TO_POINTS(Rect(7, 293, 1414, 275)));
	Sprite* HP2 = Sprite::create("HPMP.png", CC_RECT_PIXELS_TO_POINTS(Rect(1480, 57, 24, 41)));
	Sprite* MP2 = Sprite::create("HPMP.png", CC_RECT_PIXELS_TO_POINTS(Rect(1480, 184, 24, 41)));

	//使用hp条设置progressBar
	HPpt1 = ProgressTimer::create(HP1);
	HPpt1->setScale(0.25);
	HPpt1->setScaleX(11.7);
	HPpt1->setAnchorPoint(Vec2(0, 0));
	HPpt1->setType(ProgressTimerType::BAR);
	HPpt1->setBarChangeRate(Point(1, 0));
	HPpt1->setMidpoint(Point(0, 1));
	HPpt1->setPercentage(100);
	HPpt1->setPosition(Vec2(origin.x + HPpt1->getContentSize().width + 237 * 0.23 + 1, origin.y + visibleSize.height - HPMP1->getContentSize().height * 0.134));
	addChild(HPpt1, 1);
	MPpt1 = ProgressTimer::create(MP1);
	MPpt1->setScale(0.25);
	MPpt1->setScaleX(11.7);
	MPpt1->setAnchorPoint(Vec2(0, 0));
	MPpt1->setType(ProgressTimerType::BAR);
	MPpt1->setBarChangeRate(Point(1, 0));
	MPpt1->setMidpoint(Point(0, 1));
	MPpt1->setPercentage(100);
	MPpt1->setPosition(Vec2(origin.x + HPpt1->getContentSize().width + 237 * 0.23 + 1, origin.y + visibleSize.height - HPMP1->getContentSize().height * 0.24));
	addChild(MPpt1, 1);

	HPMP1->setAnchorPoint(Vec2(0, 0));
	HPMP1->setPosition(Vec2(origin.x + HPpt1->getContentSize().width * 0.5, origin.y + visibleSize.height - HPMP1->getContentSize().height * 0.3));
	HPMP1->setScale(0.25);
	addChild(HPMP1, 0);


	HPpt2 = ProgressTimer::create(HP2);
	HPpt2->setScale(0.25);
	HPpt2->setScaleX(11.7);
	HPpt2->setAnchorPoint(Vec2(0, 0));
	HPpt2->setType(ProgressTimerType::BAR);
	HPpt2->setBarChangeRate(Point(1, 0));
	HPpt2->setMidpoint(Point(1, 0));
	HPpt2->setPercentage(100);
	HPpt2->setPosition(Vec2(visibleSize.width - (HPpt2->getContentSize().width * 14.56), origin.y + visibleSize.height - HPMP2->getContentSize().height * 0.134));
	addChild(HPpt2, 1);
	MPpt2 = ProgressTimer::create(MP2);
	MPpt2->setScale(0.25);
	MPpt2->setScaleX(11.7);
	MPpt2->setAnchorPoint(Vec2(0, 0));
	MPpt2->setType(ProgressTimerType::BAR);
	MPpt2->setBarChangeRate(Point(0, 1));
	MPpt2->setMidpoint(Point(0, 1));
	MPpt2->setPercentage(100);
	MPpt2->setPosition(Vec2(visibleSize.width - (MPpt2->getContentSize().width * 14.56), origin.y + visibleSize.height - HPMP2->getContentSize().height * 0.24));
	addChild(MPpt2, 1);

	HPMP2->setAnchorPoint(Vec2(0, 0));
	HPMP2->setPosition(Vec2(visibleSize.width - (origin.x + HPpt1->getContentSize().width + HPMP2->getContentSize().width * 0.24) + 4, origin.y + visibleSize.height - HPMP2->getContentSize().height * 0.3));
	HPMP2->setScale(0.25);
	addChild(HPMP2, 0);

	/*auto hp = HPpt2->getPercentage();
	hp = (hp - 10 >= 0) ? hp - 10 : 0;
	CCProgressTo *progress = CCProgressTo::create(2, hp);
	HPpt2->runAction(progress);*///把这段代码加入需要出发增加减少血量的地方

	initAnimation();
	addKeyboardListener();
	schedule(schedule_selector(FightMode::update), 0.1f, kRepeatForever, 0);

	return true;
}
//初始化player1和player2的所有动画
void FightMode::initAnimation() {
	//创建一张玩家1的贴图
	auto texture1 = Director::getInstance()->getTextureCache()->addImage("player1/firzen_0.png");
	//从贴图中以像素单位切割，创建关键帧
	auto frame1 = SpriteFrame::createWithTexture(texture1, CC_RECT_PIXELS_TO_POINTS(Rect(81 * 6, 0, 81, 81)));
	//使用第一帧创建精灵
	player1 = Sprite::createWithSpriteFrame(frame1);
	player1->setPosition(Vec2(visibleSize.width / 4 + origin.x, origin.y + player1->getContentSize().height + 42));
	//设置缩放比例
	Size player1Size = player1->getContentSize();
	float scaleX = visibleSize.width * 0.126 / player1Size.width;
	player1->setScale(scaleX, scaleX);
	this->addChild(player1, 2);

	// player1静态动画
	player1Idle.reserve(1);
	player1Idle.pushBack(frame1);

	//创建一张玩家2的贴图
	auto texture2 = Director::getInstance()->getTextureCache()->addImage("player2/woody_0.png");
	//从贴图中以像素单位切割，创建关键帧
	auto frame2 = SpriteFrame::createWithTexture(texture2, CC_RECT_PIXELS_TO_POINTS(Rect(81 * 6, 0, 81, 81)));
	//使用第一帧创建精灵
	player2 = Sprite::createWithSpriteFrame(frame2);
	player2->setPosition(Vec2(3 * visibleSize.width / 4 + origin.x, origin.y + player2->getContentSize().height + 30));
	player2->setFlippedX(true);

	//设置缩放比例
	Size player2Size = player2->getContentSize();
	float scale2X = visibleSize.width * 0.126 / player2Size.width;
	player2->setScale(scale2X, scale2X);
	this->addChild(player2, 2);


	addKeyboardListener();
	schedule(schedule_selector(FightMode::update), 0.04f, kRepeatForever, 0);
	// player2静态动画
	player2Idle.reserve(1);
	player2Idle.pushBack(frame2);

	// player1手攻击动画
	player1AttackHand.reserve(4);
	for (int i = 0; i < 4; i++) {
		auto frame = SpriteFrame::createWithTexture(texture1, CC_RECT_PIXELS_TO_POINTS(Rect(81 * i, 81, 81, 81)));
		player1AttackHand.pushBack(frame);
	}

	//player1脚攻击动画
	player1AttackLeg.reserve(3);
	for (int i = 0; i < 3; i++) {
		auto frame = SpriteFrame::createWithTexture(texture1, CC_RECT_PIXELS_TO_POINTS(Rect(81 * i + 80 * 4, 81, 81, 81)));
		player1AttackLeg.pushBack(frame);
	}

	//player1移动动画
	player1Move.reserve(3);
	for (int i = 0; i < 3; i++) {
		auto frame = SpriteFrame::createWithTexture(texture1, CC_RECT_PIXELS_TO_POINTS(Rect(80 * i + 320, 0, 80, 81)));
		player1Move.pushBack(frame);
	}

	//player1死亡动画
	player1Dead.reserve(5);
	for (int i = 0; i < 5; i++) {
		auto frame = SpriteFrame::createWithTexture(texture1, CC_RECT_PIXELS_TO_POINTS(Rect(80 * i, 243, 80, 81)));
		player1Dead.pushBack(frame);
	}

	//player1发放气功动画
	auto texture3 = Director::getInstance()->getTextureCache()->addImage("player1/firzen_1.png");
	player1AttackQigong.reserve(3);
	for (int i = 0; i < 3; i++) {
		auto frame = SpriteFrame::createWithTexture(texture3, CC_RECT_PIXELS_TO_POINTS(Rect(79 * i, 162, 79, 81)));
		player1AttackQigong.pushBack(frame);
	}

	//player1气功形状变化动画
	auto texture4 = Director::getInstance()->getTextureCache()->addImage("player1/firzen_chasei.png");
	player1Qigong.reserve(8);
	for (int i = 0; i < 8; i++) {
		int j = 0, k = i;
		if (i >= 4) {
			j = 1;
			k = i - 4;
		}
		auto frame = SpriteFrame::createWithTexture(texture4, CC_RECT_PIXELS_TO_POINTS(Rect(78 * k, j * 80, 78, 70)));
		player1Qigong.pushBack(frame);
	}

	//player1防御动画
	player1Defense.reserve(5);
	for (int i = 0; i < 5; i++) {
		auto frame = SpriteFrame::createWithTexture(texture3, CC_RECT_PIXELS_TO_POINTS(Rect(79 * i + 79 * 3, 0, 79, 81)));
		player1Defense.pushBack(frame);
	}

	//player2手攻击动画
	auto texture5 = Director::getInstance()->getTextureCache()->addImage("player2/woody_0.png");
	player2AttackHand.reserve(8);
	for (int i = 0; i < 8; i++) {
		auto frame = SpriteFrame::createWithTexture(texture5, CC_RECT_PIXELS_TO_POINTS(Rect(80 * i, 81, 80, 81)));
		player2AttackHand.pushBack(frame);
	}
	//player2脚攻击动画
	player2AttackLeg.reserve(0);
	for (int i = 0; i < 1; i++) {
		auto frame = SpriteFrame::createWithTexture(texture5, CC_RECT_PIXELS_TO_POINTS(Rect(640, 81, 80, 81)));
		player2AttackLeg.pushBack(frame);
	}

	//player2移动动画
	player2Move.reserve(4);
	for (int i = 0; i < 4; i++) {
		auto frame = SpriteFrame::createWithTexture(texture5, CC_RECT_PIXELS_TO_POINTS(Rect(80 * i + 320, 0, 80, 81)));
		player2Move.pushBack(frame);
	}

	//player2死亡动画
	player2Dead.reserve(5);
	for (int i = 0; i < 5; i++) {
		auto frame = SpriteFrame::createWithTexture(texture5, CC_RECT_PIXELS_TO_POINTS(Rect(80 * i, 243, 80, 81)));
		player2Dead.pushBack(frame);
	}

	//player2发放气功动画
	auto texture6 = Director::getInstance()->getTextureCache()->addImage("player2/woody_2.png");
	player2AttackQigong.reserve(10);
	for (int i = 0; i < 10; i++) {
		auto frame = SpriteFrame::createWithTexture(texture6, CC_RECT_PIXELS_TO_POINTS(Rect(80 * i, 0, 80, 81)));
		player2AttackQigong.pushBack(frame);
	}

	//player2气功形状变化动画
	auto texture7 = Director::getInstance()->getTextureCache()->addImage("player2/woody_ball.png");
	player2Qigong.reserve(4);
	for (int i = 0; i < 4; i++) {
		auto frame = SpriteFrame::createWithTexture(texture7, CC_RECT_PIXELS_TO_POINTS(Rect(80 * i, 162, 80, 81)));
		player2Qigong.pushBack(frame);
	}

	//player2防御动画
	player2Defense.reserve(3);
	for (int i = 0; i < 3; i++) {
		auto frame = SpriteFrame::createWithTexture(texture5, CC_RECT_PIXELS_TO_POINTS(Rect(80 * i + 400, 405, 80, 81)));
		player2Defense.pushBack(frame);
	}
}

void FightMode::player1AttackByHand(Ref* pSender) {
	if (player1->getNumberOfRunningActions() == 0) {
		auto animation1 = Animation::createWithSpriteFrames(player1AttackHand, 0.5f);
		auto animate1 = Animate::create(animation1);
		auto animation2 = Animation::createWithSpriteFrames(player1Idle, 0.5f);
		auto animate2 = Animate::create(animation2);
		player1->runAction(Sequence::create(animate1, animate2, NULL));
	}
}

void FightMode::player1AttackByLeg(Ref* pSender) {
	if (player1->getNumberOfRunningActions() == 0) {
		auto animation1 = Animation::createWithSpriteFrames(player1AttackLeg, 0.5f);
		auto animate1 = Animate::create(animation1);
		auto animation2 = Animation::createWithSpriteFrames(player1Idle, 0.5f);
		auto animate2 = Animate::create(animation2);
		player1->runAction(Sequence::create(animate1, animate2, NULL));
	}
}

void FightMode::player1MoveAnimation() {
	if (player1->getNumberOfRunningActions() == 0) {
		auto animation1 = Animation::createWithSpriteFrames(player1Move, 0.1f);
		auto animate1 = Animate::create(animation1);
		auto animation2 = Animation::createWithSpriteFrames(player1Idle, 0.1f);
		auto animate2 = Animate::create(animation2);
		player1->runAction(Sequence::create(animate1, animate2, NULL));
	}
}

void FightMode::player1WouldDead(Ref* pSender) {
	if (player1->getNumberOfRunningActions() == 0) {
		auto animation1 = Animation::createWithSpriteFrames(player1Dead, 0.5f);
		auto animate1 = Animate::create(animation1);
		auto animation2 = Animation::createWithSpriteFrames(player1Idle, 0.5f);
		auto animate2 = Animate::create(animation2);
		player1->runAction(Sequence::create(animate1, animate2, NULL));
	}
}

void FightMode::player1WouldDefense(Ref* pSender) {
	if (player1->getNumberOfRunningActions() == 0) {
		auto animation1 = Animation::createWithSpriteFrames(player1Defense, 0.5f);
		auto animate1 = Animate::create(animation1);
		auto animation2 = Animation::createWithSpriteFrames(player1Idle, 0.5f);
		auto animate2 = Animate::create(animation2);
		player1->runAction(Sequence::create(animate1, animate2, NULL));
	}
}

//player1运行气功动画并产生气功精灵移动
void FightMode::player1AttackByQigong(Ref* pSender) {
	auto callBack = CallFunc::create(CC_CALLBACK_0(FightMode::player1QiGong, this));
	if (player1->getNumberOfRunningActions() == 0) {
		auto animation1 = Animation::createWithSpriteFrames(player1AttackQigong, 0.1f);
		auto animate1 = Animate::create(animation1);
		auto animation2 = Animation::createWithSpriteFrames(player1Idle, 0.1f);
		auto animate2 = Animate::create(animation2);
		Vector<SpriteFrame*> afterSendAnimation;
		afterSendAnimation.pushBack(player1AttackQigong.at(2));
		auto animation3 = Animation::createWithSpriteFrames(afterSendAnimation, 0.5f);
		auto animate3 = Animate::create(animation3);
		player1->runAction(Sequence::create(animate1, callBack, animate3, animate2, NULL));
	}
}

void FightMode::player1QiGong() {
	auto texture = Director::getInstance()->getTextureCache()->addImage("player1/firzen_chasei.png");
	auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 78, 70)));
	auto qigong = Sprite::createWithSpriteFrame(frame);
	qigong->setPosition(Vec2(player1->getPositionX() + 36, player1->getPositionY()));
	if (lastkey1 == 'A')
		qigong->setPosition(Vec2(player1->getPositionX() - 36, player1->getPositionY()));
	else
		qigong->setPosition(Vec2(player1->getPositionX() + 36, player1->getPositionY()));
	//设置缩放比例
	Size qigongSize = qigong->getContentSize();
	float scaleX = visibleSize.width * 0.126 / qigongSize.width;
	qigong->setScale(scaleX, scaleX);
	this->addChild(qigong, 2);
	auto animation3 = Animation::createWithSpriteFrames(player1Qigong, 0.15f);
	auto animate3 = Animate::create(animation3);
	auto moveBy = MoveBy::create(0.8, Vec2(600, 0));
	if (lastkey1 == 'A')
		moveBy = MoveBy::create(0.8, Vec2(-600, 0));
	else
		moveBy = MoveBy::create(0.8, Vec2(600, 0));
	auto spawn = Spawn::createWithTwoActions(animate3, moveBy);
	auto fadeout = FadeOut::create(0.01f);
	qigong->runAction(Sequence::create(spawn, fadeout, NULL));

}

void FightMode::player2AttackByHand(Ref* pSender) {
	if (player2->getNumberOfRunningActions() == 0) {
		auto animation1 = Animation::createWithSpriteFrames(player2AttackHand, 0.5f);
		auto animate1 = Animate::create(animation1);
		auto animation2 = Animation::createWithSpriteFrames(player2Idle, 0.5f);
		auto animate2 = Animate::create(animation2);
		player2->runAction(Sequence::create(animate1, animate2, NULL));
	}
}
void FightMode::player2AttackByLeg(Ref* pSender) {
	if (player2->getNumberOfRunningActions() == 0) {
		auto animation1 = Animation::createWithSpriteFrames(player2AttackLeg, 0.5f);
		auto animate1 = Animate::create(animation1);
		auto animation2 = Animation::createWithSpriteFrames(player2Idle, 0.5f);
		auto animate2 = Animate::create(animation2);
		player2->runAction(Sequence::create(animate1, animate2, NULL));
	}
}
void FightMode::player2MoveAnimation(Ref* pSender) {
	if (player2->getNumberOfRunningActions() == 0) {
		auto animation1 = Animation::createWithSpriteFrames(player2Move, 0.5f);
		auto animate1 = Animate::create(animation1);
		auto animation2 = Animation::createWithSpriteFrames(player2Idle, 0.5f);
		auto animate2 = Animate::create(animation2);
		player2->runAction(Sequence::create(animate1, animate2, NULL));
	}
}
void FightMode::player2WouldDead(Ref* pSender) {
	if (player2->getNumberOfRunningActions() == 0) {
		auto animation1 = Animation::createWithSpriteFrames(player2Dead, 0.5f);
		auto animate1 = Animate::create(animation1);
		auto animation2 = Animation::createWithSpriteFrames(player2Idle, 0.5f);
		auto animate2 = Animate::create(animation2);
		player2->runAction(Sequence::create(animate1, animate2, NULL));
	}
}
void FightMode::player2WouldDefense(Ref* pSender) {
	if (player2->getNumberOfRunningActions() == 0) {
		auto animation1 = Animation::createWithSpriteFrames(player2Defense, 0.5f);
		auto animate1 = Animate::create(animation1);
		auto animation2 = Animation::createWithSpriteFrames(player2Idle, 0.5f);
		auto animate2 = Animate::create(animation2);
		player2->runAction(Sequence::create(animate1, animate2, NULL));
	}
}

//player2运行气功动画并产生气功精灵移动
void FightMode::player2AttackByQigong(Ref* pSender) {
	auto callBack = CallFunc::create(CC_CALLBACK_0(FightMode::player2QiGong, this));
	if (player2->getNumberOfRunningActions() == 0) {
		auto animation1 = Animation::createWithSpriteFrames(player2AttackQigong, 0.1f);
		auto animate1 = Animate::create(animation1);
		auto animation2 = Animation::createWithSpriteFrames(player2Idle, 0.1f);
		auto animate2 = Animate::create(animation2);
		Vector<SpriteFrame*> afterSendAnimation;
		afterSendAnimation.pushBack(player2AttackQigong.at(8));
		auto animation3 = Animation::createWithSpriteFrames(afterSendAnimation, 0.3f);
		auto animate3 = Animate::create(animation3);
		player2->runAction(Sequence::create(animate1, callBack, animate3, animate2, NULL));
	}
}
void FightMode::player2QiGong() {
	auto texture = Director::getInstance()->getTextureCache()->addImage("player2/woody_ball.png");
	auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 162, 80, 81)));
	auto qigong = Sprite::createWithSpriteFrame(frame);
	qigong->setPosition(Vec2(player2->getPositionX() + 36, player2->getPositionY()));
	if (lastkey2 == 'A') {
		qigong->setPosition(Vec2(player2->getPositionX() - 36, player2->getPositionY()));
		qigong->setFlippedX(true);
	}
	else {
		qigong->setPosition(Vec2(player2->getPositionX() + 36, player2->getPositionY()));
		qigong->setFlippedX(false);
	}
	//设置缩放比例
	Size qigongSize = qigong->getContentSize();
	float scaleX = visibleSize.width * 0.126 / qigongSize.width;
	qigong->setScale(scaleX, scaleX);
	this->addChild(qigong, 2);
	auto animation3 = Animation::createWithSpriteFrames(player2Qigong, 0.3f);
	auto animate3 = Animate::create(animation3);
	auto moveBy = MoveBy::create(1.2, Vec2(600, 0));
	if (lastkey2 == 'A')
		moveBy = MoveBy::create(1.2, Vec2(-600, 0));
	else
		moveBy = MoveBy::create(1.2, Vec2(600, 0));
	auto spawn = Spawn::createWithTwoActions(animate3, moveBy);
	auto fadeout = FadeOut::create(0.01f);
	qigong->runAction(Sequence::create(spawn, fadeout, NULL));
}

void FightMode::update(float f) {
	if (player1ADIsMove || player1WSIsMove) {
		this->player1Movement(player1ADMovekey, player1WSMovekey);
	}
	if (player2ADIsMove || player2WSIsMove) {
		this->player2Movement(player2ADMovekey, player2WSMovekey);
	}
}

//人物移动函数
void FightMode::addKeyboardListener() {
	auto keyboardListener1 = EventListenerKeyboard::create();
	keyboardListener1->onKeyPressed = CC_CALLBACK_2(FightMode::onKeyPressed1, this);
	keyboardListener1->onKeyReleased = CC_CALLBACK_2(FightMode::onKeyReleased1, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener1, player1);

	auto keyboardListener2 = EventListenerKeyboard::create();
	keyboardListener2->onKeyPressed = CC_CALLBACK_2(FightMode::onKeyPressed2, this);
	keyboardListener2->onKeyReleased = CC_CALLBACK_2(FightMode::onKeyReleased2, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener2, player2);
}


void FightMode::onKeyPressed1(EventKeyboard::KeyCode code, Event* event) {
	switch (code)
	{
	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_A:
	case cocos2d::EventKeyboard::KeyCode::KEY_A:
		player1ADIsMove = true;
		player1ADMovekey = 'A';
		if (lastkey1 == 'D') {
			player1->setFlippedX(true);
		}
		lastkey1 = 'A';
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_D:
	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_D:
		player1ADIsMove = true;
		player1ADMovekey = 'D';
		if (lastkey1 == 'A') {
			player1->setFlippedX(false);
		}
		lastkey1 = 'D';
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_W:
	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_W:
		player1WSIsMove = true;
		player1WSMovekey = 'W';
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_S:
	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_S:
		player1WSIsMove = true;
		player1WSMovekey = 'S';
		break;
	}
}
void FightMode::onKeyReleased1(EventKeyboard::KeyCode code, Event *event) {
	switch (code)
	{
	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_A:
	case cocos2d::EventKeyboard::KeyCode::KEY_A:
	case cocos2d::EventKeyboard::KeyCode::KEY_D:
	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_D:
		player1ADIsMove = false;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_W:
	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_W:
	case cocos2d::EventKeyboard::KeyCode::KEY_S:
	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_S:
		player1WSIsMove = false;
		break;
	}
}
void FightMode::player1Movement(char ADkey, char WSkey) {
	float x_move = 0;
	float y_move = 0;
	if (player1ADIsMove) {
		switch (ADkey)
		{
		case 'A':
			if (player1->getPositionX() - x_num >= 30) {
				x_move = 0 - x_num;
			}
			else {
				x_move = 30 - player1->getPositionX();
			}
			break;
		case 'D':
			if (player1->getPositionX() + x_num <= visibleSize.width - 30) {
				x_move = x_num;
			}
			else {
				x_move = visibleSize.width - 30 - player1->getPositionX();
			}
			break;
		}
	}
	if (player1WSIsMove) {
		switch (WSkey) {
		case 'W':
			if (player1->getPositionY() + y_num <= 280) {
				y_move = y_num;
			}
			else {
				y_move = 280 - player1->getPositionY();
			}
			break;
		case 'S':
			if (player1->getPositionY() - y_num >= 80) {
				y_move = 0 - y_num;
			}
			else {
				y_move = 80 - player1->getPositionY();
			}
			break;
		}
	}
	auto move = MoveBy::create(0.1f, Vec2(x_move, y_move));
	auto animation1 = Animation::createWithSpriteFrames(player1Move, 0.10f);
	auto animate1 = Animate::create(animation1);
	auto animation2 = Animation::createWithSpriteFrames(player1Idle, 0.10f);
	auto animate2 = Animate::create(animation2);
	player1->runAction(Sequence::create(Spawn::create(animate1, move, NULL), animate2, NULL));
}


void FightMode::onKeyPressed2(EventKeyboard::KeyCode code, Event* event) {
	switch (code)
	{
	case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		player2ADIsMove = true;
		player2ADMovekey = 'A';
		if (lastkey2 == 'D') {
			player2->setFlippedX(true);
		}
		lastkey2 = 'A';
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		player2ADIsMove = true;
		player2ADMovekey = 'D';
		if (lastkey2 == 'A') {
			player2->setFlippedX(false);
		}
		lastkey2 = 'D';
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
		player2WSIsMove = true;
		player2WSMovekey = 'W';
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		player2WSIsMove = true;
		player2WSMovekey = 'S';
		break;
	}
}
void FightMode::onKeyReleased2(EventKeyboard::KeyCode code, Event *event) {
	switch (code)
	{
	case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
	case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		player2ADIsMove = false;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
	case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		player2WSIsMove = false;
		break;
	}
}
void FightMode::player2Movement(char ADkey, char WSkey) {
	float x_move = 0;
	float y_move = 0;
	if (player2ADIsMove) {
		switch (ADkey)
		{
		case 'A':
			if (player2->getPositionX() - x_num >= 30) {
				x_move = 0 - x_num;
			}
			else {
				x_move = 30 - player2->getPositionX();
			}
			break;
		case 'D':
			if (player2->getPositionX() + x_num <= visibleSize.width - 30) {
				x_move = x_num;
			}
			else {
				x_move = visibleSize.width - 30 - player2->getPositionX();
			}
			break;
		}
	}
	if (player2WSIsMove) {
		switch (WSkey) {
		case 'W':
			if (player2->getPositionY() + y_num <= 280) {
				y_move = y_num;
			}
			else {
				y_move = 280 - player2->getPositionY();
			}
			break;
		case 'S':
			if (player2->getPositionY() - y_num >= 80) {
				y_move = 0 - y_num;
			}
			else {
				y_move = 80 - player2->getPositionY();
			}
			break;
		}
	}
	auto move = MoveBy::create(0.1f, Vec2(x_move, y_move));
	auto animation1 = Animation::createWithSpriteFrames(player2Move, 0.1f);
	auto animate1 = Animate::create(animation1);
	auto animation2 = Animation::createWithSpriteFrames(player2Idle, 0.1f);
	auto animate2 = Animate::create(animation2);
	player2->runAction(Sequence::create(Spawn::create(animate1, move, NULL), animate2, NULL));
}