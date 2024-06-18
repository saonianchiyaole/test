/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "Scene/GameScene.h"
#include "Scene/GameOverScene.h"
#include "Scene/PauseScene.h"

GameScene* GameScene::instance = nullptr;

USING_NS_CC;

Scene* GameScene::createScene()
{
	//auto scene= GameScene::createWithPhysics();

	return GameScene::create();
}


// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in GameScene.cpp\n");
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::initWithPhysics())
	{
		return false;
	}

	// 启用物理引擎debug功能
	this->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);


	////////////////////////////
	//2.在此处加入填写初始化场景属性的代码
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// player
	player = Player::create(Vec2(visibleSize.width / 4, 23));
	this->addChild(player, player->localZOrder);

	//// enemy
	//Enemy* newEnemy = Enemy::create(Vec2(visibleSize.width - 200, 150), "EnemyAnimation/wizardMove");
	//enemyByHashSet.insert(newEnemy);
	//this->addChild(newEnemy, newEnemy->localZOrder);

	////////////////////////////


	//初始化变量
	initVariable();

	//获取鼠标位置侦听器
	addMouseMoveListener();

	//添加碰撞检测侦听器
	addContactListener();

	//添加波次管理器
	//addWaveManager();

	//初始化按键监控
	initKeys();

	//初始化按键监听器
	addKeyBoardListener();

	//初始化组件
	initComponent();

	//着色器
	initRedShader();

	drawSelectWeaponButton();




	//将update加入schedule
	this->scheduleUpdate();


	return true;
}

void GameScene::update(float dt) {
	updateTimer();
	updateOptions();
}


void GameScene::gameoverCallback(Ref* pSender)
{
	this->unscheduleUpdate();
	this->removeAllChildrenWithCleanup(true);
	this->removeFromParentAndCleanup(true);
	Director::getInstance()->replaceScene(GameOverScene::createScene());

}

GameScene* GameScene::getInstance() {
	if (instance == nullptr) {
		instance = new GameScene;
		//scene = Scene::createWithPhysics();
	}
	return instance;
}

void GameScene::onMouseMove(Event* event) {
	EventMouse* e = (EventMouse*)event;
	mousePosition = Vec2(e->getCursorX(), e->getCursorY());
}

void GameScene::addMouseMoveListener() {
	//固定格式
	auto mouseListener = EventListenerMouse::create();
	mouseListener->onMouseMove = CC_CALLBACK_1(GameScene::onMouseMove, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
}
bool GameScene::onContactBegin(PhysicsContact& contact) {
	// 处理碰撞事件
	auto shapeA = contact.getShapeA();
	auto shapeB = contact.getShapeB();
	auto bodyA = shapeA->getBody();
	auto bodyB = shapeB->getBody();
	//获得对象的基类指针
	auto nodeA = bodyA->getNode();
	auto nodeB = bodyB->getNode();
	//获得掩码
	int categoryA = bodyA->getCategoryBitmask();
	int categoryB = bodyB->getCategoryBitmask();
	//具体处理碰撞的部分
	if ((categoryA == CategoryBitMask::PLAYER && categoryB == CategoryBitMask::ENEMY) ||
		(categoryA == CategoryBitMask::ENEMY && categoryB == CategoryBitMask::PLAYER)) {
		// Arrow collided with another object
		if (categoryA == CategoryBitMask::ENEMY) {
			//获取player和enemy对应的指针
			auto player = dynamic_cast<Player*>(nodeB);
			auto enemy = dynamic_cast<Enemy*>(nodeA);
			if (enemy && player && player->m_isAlive) {
				player->hurt(enemy->attackCapacity);
			}
		}
		else if (categoryB == CategoryBitMask::ENEMY) {
			auto player = dynamic_cast<Player*>(nodeA);
			auto enemy = dynamic_cast<Enemy*>(nodeB);
			if (enemy && player && player->m_isAlive) {
				player->hurt(enemy->attackCapacity);
			}
		}
	}

	return true;
}

void GameScene::addContactListener() {
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(GameScene::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
}

void GameScene::addWaveManager() {
	if (waveManager != nullptr)
		return;
	waveManager = WaveManager::create();
	this->addChild(waveManager);
}

void GameScene::initVariable() {

	//时间相关
	timeManager = TimeManager::getInstance();
	timeManager->ifInGameScene = true;
	//一局最大时长
	maxTimePerGame = 5.f * 60.f;

	//BUFF缓冲区相关
	isAddButtonOk = true;
	buttonBufferNum = 0;

	waveManager = nullptr;

	isDisplayingOption = false;

	isGameOver = false;
	havePlayerWin = false;
	isGameStop = false;
	//先让主角选把武器
	havePlayerSelectWeapon = false;

}
void GameScene::displayOptions() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	

	//玩家buff部分
	buff_player = new Buff_Player();
	auto item = Button::create(buff_player->path, buff_player->path);

	item->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED) {
			buff = buff_player;
			buffCallback();
			
		}
		});

	float x = origin.x + visibleSize.width / 2 - 100;
	float y = origin.y + item->getContentSize().height / 2 + 120;
	item->setPosition(Vec2(x, y));
	item->setTag(TAG_OF_BUFF_PLAYER);

	auto label = Label::createWithTTF(buff_player->tips, "fonts/fzkt.ttf", 20);
	label->setPosition(Vec2(x, y - item->getContentSize().height / 2 - label->getContentSize().height / 2));
	GameScene::getInstance()->addChild(label, 5);
	label->setTag(TAG_OF_DESCRIPTION_OF_BUFF_PLAYER);

	this->addChild(item, 1);

	//武器buff部分
	buff_weapon = new Buff_Weapon();
	item = Button::create(buff_weapon->path, buff_weapon->path);

	item->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED) {
			buff = buff_weapon;
			buffCallback();
			
		}
		});

	x = origin.x + visibleSize.width / 2 + 100;
	y = origin.y + item->getContentSize().height / 2 + 120;
	item->setPosition(Vec2(x, y));
	item->setTag(TAG_OF_BUFF_WEAPON);

	label = Label::createWithTTF(buff_weapon->tips, "fonts/fzkt.ttf", 20);
	label->setPosition(Vec2(x, y - item->getContentSize().height / 2 - label->getContentSize().height / 2));
	GameScene::getInstance()->addChild(label, 5);
	label->setTag(TAG_OF_DESCRIPTION_OF_BUFF_WEAPON);

	this->addChild(item, 1);
}

void GameScene::addActionToOption(int tag) {
	auto button = this->getChildByTag(tag);

	button->runAction(Sequence::create(
		Spawn::create(MoveBy::create(0.7f, Vec2(0, 30)),
			FadeOut::create(0.7f),
			nullptr),
		CallFunc::create(CC_CALLBACK_0(GameScene::flyend, this)),
		NULL
	));
}

void GameScene::buffCallback() {
	buff->addBuff();

	if (buff == buff_player) {
		GameScene::addActionToOption(TAG_OF_BUFF_PLAYER);
		GameScene::addActionToOption(TAG_OF_DESCRIPTION_OF_BUFF_PLAYER);
	}

	if (buff == buff_weapon) {
		GameScene::addActionToOption(TAG_OF_BUFF_WEAPON);
		GameScene::addActionToOption(TAG_OF_DESCRIPTION_OF_BUFF_WEAPON);
	}
}

void GameScene::flyend() {
	auto button = this->getChildByTag(TAG_OF_BUFF_PLAYER);
	//会被调用多次，仅第一次生效
	if (button == nullptr)
		return;
	button->setVisible(false);
	button->removeFromParent();
	auto label = this->getChildByTag(TAG_OF_DESCRIPTION_OF_BUFF_PLAYER);
	label->setVisible(false);
	label->removeFromParent();

	button = this->getChildByTag(TAG_OF_BUFF_WEAPON);
	button->setVisible(false);
	button->removeFromParent();
	label = this->getChildByTag(TAG_OF_DESCRIPTION_OF_BUFF_WEAPON);
	label->setVisible(false);
	label->removeFromParent();
}
bool GameScene::addEnemy(Enemy* newEnemy) {
	if (enemyByHashSet.find(newEnemy) != enemyByHashSet.end()) {
		return false;
	}
	enemyByHashSet.insert(newEnemy);
	this->addChild(newEnemy);
	return true;
}
bool GameScene::removeEnemy(Enemy* enemy) {
	if (enemyByHashSet.find(enemy) != enemyByHashSet.end())
	{
		enemyByHashSet.erase(enemy);
		enemy->dead();
		return true;
	}
	return false;
}
bool GameScene::removeEnemyFromHashSet(Enemy* enemy) {
	if (enemyByHashSet.find(enemy) != enemyByHashSet.end()) {
		enemyByHashSet.erase(enemy);
		return true;
	}
	return false;
}
void GameScene::pauseGame() {
	// 暂停游戏
	Director::getInstance()->pushScene(PauseScene::createScene());
}

inline void GameScene::initComponentPointer() {
	waveNumLabel = nullptr;
	timerLabel = nullptr;
}
void GameScene::initComponent() {
	initComponentPointer();

	timeManager->setInGameTime(0.0f);

	//计时器
	updateTimer();
	//波次统计
	updateWaveNum(0);

	//添加关闭按钮
	addCloseItem();

	//添加背景
	addBackground();

}

inline void GameScene::drawTimer(int minute, int second) {
	if (timerLabel) {
		timerLabel->setString(std::to_string(minute) + ":" + std::to_string(second));
		return;
	}
	Vec2 visibleSize = Director::getInstance()->getVisibleSize();
	timerLabel = Label::createWithTTF(std::to_string(minute) + ":" + std::to_string(second), "fonts/fzkt.ttf", 20);
	timerLabel->setPosition(Vec2(visibleSize.x / 2, visibleSize.y - timerLabel->getContentSize().height / 2));
	timerLabel->setTag(GAME_SCENE_TIMER_TAG);
	this->addChild(timerLabel, GAME_SCENE_TIMER_LAYER);
}
void GameScene::drawWaveNum(int num) {
	if (waveNumLabel) {
		waveNumLabel->setString("Wave" + std::to_string(num));
		return;
	}
	Vec2 visibleSize = Director::getInstance()->getVisibleSize();
	waveNumLabel = Label::createWithTTF("Wave" + std::to_string(num), "fonts/fzkt.ttf", 20);
	waveNumLabel->setPosition(Vec2(visibleSize.x / 2, visibleSize.y - waveNumLabel->getContentSize().height / 2 - timerLabel->getContentSize().width));
	waveNumLabel->setTag(GAME_SCENE_WAVE_NUM_TAG);

	this->addChild(waveNumLabel, GAME_SCENE_WAVE_NUM_LAYER);
}
inline void GameScene::updateTimer() {

	int remainedTimeExpressedBySecond = maxTimePerGame - timeManager->getInGameTime();
	int min = remainedTimeExpressedBySecond / 60;
	int sec = remainedTimeExpressedBySecond % 60;

	if (min == 0 && sec == 0)
		gameOver(true);
	drawTimer(min, sec);
}
void GameScene::updateWaveNum(int num) {
	//更新波次
	//绘制波次
	drawWaveNum(num);
}

void GameScene::initKeys() {
	//key监控
	keys.clear();
	keys.emplace(EventKeyboard::KeyCode::KEY_W, false);
	keys.emplace(EventKeyboard::KeyCode::KEY_A, false);
	keys.emplace(EventKeyboard::KeyCode::KEY_S, false);
	keys.emplace(EventKeyboard::KeyCode::KEY_D, false);
}
void GameScene::addKeyBoardListener() {
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event* event) {
		if (keyCode == EventKeyboard::KeyCode::KEY_SPACE) {
			pauseGame();
		}
		else if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE) {
			Director::getInstance()->pushScene(PauseScene::createScene());
		}
		};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
}

void GameScene::addCloseItem() {

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(GameScene::gameoverCallback, this));

	if (closeItem == nullptr ||
		closeItem->getContentSize().width <= 0 ||
		closeItem->getContentSize().height <= 0)
	{
		problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
	}
	else
	{
		float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
		float y = origin.y + closeItem->getContentSize().height / 2;
		closeItem->setPosition(Vec2(x, y));
	}

	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
}
void GameScene::addBackground() {

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto background = Sprite::create(u8"背景-大.png");
	background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(background, 0);

	// 获取精灵的原始大小
	auto spriteSize = background->getContentSize();

	// 计算缩放比例
	float scaleX = designResolutionSize.width / spriteSize.width;
	float scaleY = designResolutionSize.height / spriteSize.height;

	// 设置精灵的缩放比例
	background->setScaleX(scaleX);
	background->setScaleY(scaleY);
}

void GameScene::updateOptions() {
	//当缓存区还有按钮 且 按钮已经被点下时，展示新的按钮
	if (buttonBufferNum > 0 && isAddButtonOk) {
		isAddButtonOk = false;
		buttonBufferNum--;
		GameScene::getInstance()->displayOptions();
	}
	//当按钮消失后，可以添加新的按钮
	vector<int> tags = { TAG_OF_BUFF_PLAYER, TAG_OF_DESCRIPTION_OF_BUFF_PLAYER,
						TAG_OF_BUFF_WEAPON, TAG_OF_DESCRIPTION_OF_BUFF_WEAPON };
	for (int tag : tags) {
		if (this->getChildByTag(tag))
			return;
	}
	isAddButtonOk = true;
}

void GameScene::initRedShader() {
	// 加载自定义着色器
	auto glprogram = GLProgram::createWithFilenames("Shader/redShader.vsh", "Shader/redShader.fsh");
	auto glprogramstate = GLProgramState::getOrCreateWithGLProgram(glprogram);

	//player->setBlendFunc(BlendFunc::ALPHA_PREMULTIPLIED);
	//player->setGLProgramState(glprogramstate);
}

void GameScene::gameOver(bool winOrNot) {
	if (winOrNot == true)
		havePlayerWin = true;
	else havePlayerWin = false;

	isGameOver = false;

	this->unscheduleUpdate();
	this->removeAllChildrenWithCleanup(true);
	this->removeFromParentAndCleanup(true);
	Director::getInstance()->replaceScene(GameOverScene::createScene());


}
void GameScene::setInstanceToNullptr() {
	instance = nullptr;
}

void GameScene::drawSelectWeaponButton() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	isAddButtonOk = false;

	//玩家buff部分
	auto item = Button::create("WeaponResource/bow.png", "WeaponResource/bow.png");

	item->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED) {
			player->addWeapon("bow");
			havePlayerSelectWeapon = true;
			addWaveManager();
			GameScene::addActionToOption(TAG_OF_BUFF_PLAYER);
			GameScene::addActionToOption(TAG_OF_DESCRIPTION_OF_BUFF_PLAYER);
			isAddButtonOk = true;
			waveManager->canSelectWeapon = false;
		}
		});

	float x = origin.x + visibleSize.width / 2 - 100;
	float y = origin.y + item->getContentSize().height / 2 + 120;
	item->setPosition(Vec2(x, y));
	item->setTag(TAG_OF_BUFF_PLAYER);

	auto label = Label::createWithTTF(u8"弓箭", "fonts/fzkt.ttf", 20);
	label->setPosition(Vec2(x, y - item->getContentSize().height / 2 - label->getContentSize().height / 2));
	GameScene::getInstance()->addChild(label, 5);
	label->setTag(TAG_OF_DESCRIPTION_OF_BUFF_PLAYER);

	this->addChild(item, 1);

	//武器buff部分
	item = Button::create("WeaponResource/sword.png", "WeaponResource/sword.png");

	item->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED) {
			player->addWeapon("Sword");
			havePlayerSelectWeapon = true;
			addWaveManager();
			GameScene::addActionToOption(TAG_OF_BUFF_WEAPON);
			GameScene::addActionToOption(TAG_OF_DESCRIPTION_OF_BUFF_WEAPON);
			isAddButtonOk = true;
			waveManager->canSelectWeapon = false;
		}
		});

	x = origin.x + visibleSize.width / 2 + 100;
	y = origin.y + item->getContentSize().height / 2 + 120;
	item->setPosition(Vec2(x, y));
	item->setTag(TAG_OF_BUFF_WEAPON);

	label = Label::createWithTTF(u8"剑", "fonts/fzkt.ttf", 20);
	label->setPosition(Vec2(x, y - item->getContentSize().height / 2 - label->getContentSize().height / 2));
	GameScene::getInstance()->addChild(label, 5);
	label->setTag(TAG_OF_DESCRIPTION_OF_BUFF_WEAPON);

	this->addChild(item, 1);
}