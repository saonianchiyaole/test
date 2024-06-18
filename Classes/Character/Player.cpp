#include "Character/Player.h"
#include "Scene/GameScene.h"

Player::Player() :Character()
{
	//初始化
	//移动相关
	horizontalSpeed = 0;
	verticalSpeed = 0;
	horizontalState = NOTHING;
	verticalState = NOTHING;

	//等级、经验相关
	level = 1;
	exp = 0;

	//lives_num = LIVES_OF_Player;
	//isMagicAttack = false;
	/*m_isrunning = false;
	m_isdead = false;
	m_isAttack = false;
	m_ishurt = false;
	m_max_life = 500;
	m_life = m_max_life;

	m_characterState = LOADING;*/
}

/*Player::~Player()
{

}*/

Player* Player::create(Vec2 position) {
	Player* pRet = new(std::nothrow) Player();
	if (pRet && pRet->init(position)) {
		pRet->autorelease();
		return pRet;
	}
	else {
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

bool Player::init(Vec2 position)
{
	if (!Sprite::init())
	{
		return false;
	}

	// 暂用原来的动画
	/*ArmatureDataManager::getInstance()->addArmatureFileInfo(PATH_OF_ARMATURE_OF_PLAYER);
	m_armature = Armature::create("Hero");
	if (m_armature == NULL)
	{
		CCLOG("Player load error!");
		return false;
	}
	m_armature->setPosition(Vec2::ZERO);
	m_armature->getAnimation()->play("loading");
	m_armature->getAnimation()->setFrameEventCallFunc(CC_CALLBACK_0(Player::onFrameEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
	this->addChild(m_armature);*/
	this->setPosition(position);

	this->scheduleUpdate();
	//数值初始化
	m_isrunning = false;
	m_isdead = false;
	m_isAttack = false;
	m_ishurt = false;
	maxHP = 100;
	curHP = maxHP;
	m_isAlive = true;
	//攻击 暴击 防御
	attackCapacity = 5;
	criticalHitRate = 0;
	defendCapacity = 0;


	this->setTexture("PlayerAnimation/playerMove3.png");
	//添加行走动画

	createWalkAction("PlayerAnimation/playerMove", 0, 3, 0.27f);

	//添加武器
	//addWeapon("bow");
	//addWeapon("Sword");

	//物理引擎
	initPhysicsBody();

	//添加血条
	initBloodBar();

	initExpBar();

	//添加键盘监听接口
	addKeyboardMoveEvents();
	//
	keys = &(GameScene::getInstance()->keys);

	return true;
}
void Player::addWeapon(std::string weaponType) {
	//Weapon* newWeapon = ;
	auto weapon = WeaponFactory::addWeapon(weaponType);
	this->addChild(weapon);
	weaponsPlayerOwn.push_back(weapon);
}

//Player控制移动部分，已实现，没问题，可以不看
//使用WSAD四个按键控制上下左右
void Player::addKeyboardMoveEvents() {
	auto eventListener = EventListenerKeyboard::create();

	eventListener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event* event) {
		switch (keyCode) {
		case EventKeyboard::KeyCode::KEY_W:
			if (verticalState == NOTHING) {//未按下任何按键
				verticalState = UP;
			}
			else if (verticalState == DOWN) {//已按下相对按键
				verticalState = SUPERPOSITION;
			}
			keys->find(EventKeyboard::KeyCode::KEY_W)->second = true;
			break;
		case EventKeyboard::KeyCode::KEY_S:
			if (verticalState == NOTHING) {//未按下任何按键
				verticalState = DOWN;
			}
			else if (verticalState == UP) {//已按下相对按键
				verticalState = SUPERPOSITION;
			}
			keys->find(EventKeyboard::KeyCode::KEY_S)->second = true;
			break;
		case EventKeyboard::KeyCode::KEY_D:
			if (horizontalState == NOTHING) {//未按下任何按键
				horizontalState = UP;
			}
			else if (horizontalState == DOWN) {//已按下相对按键
				horizontalState = SUPERPOSITION;
			}
			keys->find(EventKeyboard::KeyCode::KEY_D)->second = true;
			break;
		case EventKeyboard::KeyCode::KEY_A:
			if (horizontalState == NOTHING) {//未按下任何按键
				horizontalState = DOWN;
			}
			else if (horizontalState == UP) {//已按下相对按键
				horizontalState = SUPERPOSITION;
			}
			keys->find(EventKeyboard::KeyCode::KEY_A)->second = true;
			break;
		default:
			break;
		}
		};

	eventListener->onKeyReleased = [this](EventKeyboard::KeyCode keyCode, Event* event) {
		switch (keyCode) {
		case EventKeyboard::KeyCode::KEY_W:
			if (verticalState == SUPERPOSITION) {//叠加态
				verticalState = DOWN;
			}
			else if (verticalState == UP) {//仅按下一个按键
				verticalState = NOTHING;
			}
			keys->find(EventKeyboard::KeyCode::KEY_W)->second = false;
			break;
		case EventKeyboard::KeyCode::KEY_S:
			if (verticalState == SUPERPOSITION) {//叠加态
				verticalState = UP;
			}
			else if (verticalState == DOWN) {//仅按下一个按键
				verticalState = NOTHING;
			}
			keys->find(EventKeyboard::KeyCode::KEY_S)->second = false;
			break;
		case EventKeyboard::KeyCode::KEY_D:
			if (horizontalState == SUPERPOSITION) {//叠加态
				horizontalState = DOWN;
			}
			else if (horizontalState == UP) {//仅按下一个按键
				horizontalState = NOTHING;
			}
			keys->find(EventKeyboard::KeyCode::KEY_D)->second = false;
			break;
		case EventKeyboard::KeyCode::KEY_A:
			if (horizontalState == SUPERPOSITION) {//叠加态
				horizontalState = UP;
			}
			else if (horizontalState == DOWN) {//仅按下一个按键
				horizontalState = NOTHING;
			}
			keys->find(EventKeyboard::KeyCode::KEY_A)->second = false;
			break;
		default:
			break;
		}
		};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, this);
}

void Player::updatePosition(float dt) {
	if (verticalState == UP) {
		verticalSpeed = min(MAX_SPEED, verticalSpeed + velocityDelta);
	}
	else if (verticalState == DOWN) {
		verticalSpeed = max(-MAX_SPEED, verticalSpeed - velocityDelta);
	}
	else {
		if (verticalSpeed < 0) {
			verticalSpeed = min(MAX_SPEED, verticalSpeed + velocityDelta);
		}
		else if (verticalSpeed > 0) {
			verticalSpeed = max(-MAX_SPEED, verticalSpeed - velocityDelta);
		}
	}
	if (horizontalState == UP) {
		this->setScaleX(1.0f);
		horizontalSpeed = min(MAX_SPEED, horizontalSpeed + velocityDelta);
	}
	else if (horizontalState == DOWN) {
		this->setScaleX(-1.0f);
		horizontalSpeed = max(-MAX_SPEED, horizontalSpeed - velocityDelta);
	}
	else {
		if (horizontalSpeed < 0) {
			this->setScaleX(-1.0f);
			horizontalSpeed = min(MAX_SPEED, horizontalSpeed + velocityDelta);
		}
		else if (horizontalSpeed > 0) {
			this->setScaleX(1.0f);
			horizontalSpeed = max(-MAX_SPEED, horizontalSpeed - velocityDelta);
		}
	}
	//乘以dt为了适应不同帧率
	//setPosition(getPosition() + Vec2(horizontalSpeed * dt, verticalSpeed * dt));

	auto visibleSize = Director::getInstance()->getVisibleSize();
	// 乘以dt为了适应不同帧率
	auto newpos = getPosition() + Vec2(horizontalSpeed * dt, verticalSpeed * dt);
	//检测越界
	if (newpos.x<0 || newpos.x >visibleSize.width || newpos.y<0 || newpos.y >visibleSize.height)
		return;
	setPosition(newpos);
}

void Player::update(float dt)
{
	checkKeys();
	updatePosition(dt);
	callWeaponAttack();
}
void Player::checkKeys() {
	if (keys->size() == 0)
		return;
	if (keys->find(EventKeyboard::KeyCode::KEY_A)->second == true
		|| keys->find(EventKeyboard::KeyCode::KEY_W)->second == true
		|| keys->find(EventKeyboard::KeyCode::KEY_S)->second == true
		|| keys->find(EventKeyboard::KeyCode::KEY_D)->second == true
		)
	{
		if (this->getActionByTag(PLAYER_MOVE_TAG) == nullptr)
			this->runAction(moveAction);
	}
	else if (this->getActionByTag(PLAYER_MOVE_TAG) != nullptr) {
		this->stopActionByTag(PLAYER_MOVE_TAG);
	}
}
Animation* Player::createWalkAnimation(const std::string& prefixName, int start, int end, float delay) {
	Vector<SpriteFrame*> animFrames;
	for (int i = start; i <= end; ++i) {
		std::string frameName = prefixName + std::to_string(i) + ".png";
		auto frame = SpriteFrame::create(frameName, Rect(0, 0, 140, 140)); // 假设每个图片的大小是100x100
		if (frame) {
			animFrames.pushBack(frame);
		}
	}
	return Animation::createWithSpriteFrames(animFrames, delay);
}

void Player::callWeaponAttack() {
	if (weaponsPlayerOwn.size() == 0)
		return;
	for (auto weapon : weaponsPlayerOwn) {
		weapon->attack();
	}
}

void Player::hurt(int damage) {
	if (!m_isAlive)
		false;
	curHP -= std::max(0, damage - int(defendCapacity));
	//更新血条
	updateBloodBar(damage);
	if (curHP <= 0) {
		m_isAlive = false;
		this->dead();
	}
}
//线性函数，1级升级需100点经验，每升一级加50经验
float Player::getMaxExp() {
	return 50 * level + 50;
}
//处理升级事务
void Player::processExp(float delta_exp) {
	exp += delta_exp;
	//可能连升很多级，比如打boss
	while (exp >= getMaxExp()) {
		exp -= getMaxExp();
		level++;
		GameScene::getInstance()->buttonBufferNum++;
	}
	updateExpBar(exp, getMaxExp());
}
void Player::initPhysicsBody() {
	// 创建物理体
	Size boundingBox = this->getContentSize();
	auto physicsBody = PhysicsBody::createBox(boundingBox); // 假设箭头部分的碰撞框大小为10x10
	physicsBody->setGravityEnable(false);
	physicsBody->setDynamic(true); // 设置为动态物理体
	physicsBody->setCategoryBitmask(CategoryBitMask::PLAYER); // 设置分类掩码
	physicsBody->setCollisionBitmask(0x00000000); // 设置碰撞掩码
	physicsBody->setContactTestBitmask(CategoryBitMask::ENEMY); // 设置接触测试掩码
	this->setPhysicsBody(physicsBody);
}

void Player::initExpBar() {

	Vec2 VisibleSize = Director::getInstance()->getVisibleSize();

	expBarEmpty = Sprite::create(PATH_OF_EXP_BAR_EMPTY);
	Size expBarPlace = { expBarEmpty->getContentSize().width * PLAYER_BAR_ENLARGE_RATE - 80 ,VisibleSize.y - expBarEmpty->getContentSize().height * PLAYER_BAR_ENLARGE_RATE - expBarEmpty->getContentSize().height * PLAYER_BAR_ENLARGE_RATE - 20 };
	expBarEmpty->setPosition(expBarPlace);

	expBar = Sprite::create(PATH_OF_EXP_BAR);
	expBar->setPosition(expBarPlace);

	//Player��Ѫ���;�������Ҫ��һ��
	expBar->setScale(PLAYER_BAR_ENLARGE_RATE);
	expBarEmpty->setScale(PLAYER_BAR_ENLARGE_RATE);
	expBar->setScaleX(0); // һ��ʼû���飬����Ѫ��

	GameScene::getInstance()->addChild(expBarEmpty, 3);
	GameScene::getInstance()->addChild(expBar, 4);
}

void Player::initBloodBar() {

	Vec2 VisibleSize = Director::getInstance()->getVisibleSize();

	bloodBarEmpty = Sprite::create(PATH_OF_BLOOD_BAR_EMPTY);
	Size bloodBarPlace = { bloodBarEmpty->getContentSize().width * PLAYER_BAR_ENLARGE_RATE - 80 ,VisibleSize.y - bloodBarEmpty->getContentSize().height * PLAYER_BAR_ENLARGE_RATE };
	bloodBarEmpty->setPosition(bloodBarPlace);

	bloodBar = Sprite::create(PATH_OF_BLOOD_BAR);
	bloodBar->setPosition(bloodBarPlace);

	//Player��Ѫ���;�������Ҫ��һ��
	bloodBarEmpty->setScale(PLAYER_BAR_ENLARGE_RATE);
	bloodBar->setScale(PLAYER_BAR_ENLARGE_RATE);

	GameScene::getInstance()->addChild(bloodBarEmpty, 3);
	GameScene::getInstance()->addChild(bloodBar, 4);
}
void Player::dead() {
	this->m_isAlive = false;
	GameScene::getInstance()->gameOver(false);
	//this->removeFromParent();

}
void Player::updateBloodBar(float damage) {
	if (curHP <= 0) {
		bloodBar->setScaleX(0);
	}
	bloodBar->setScaleX(curHP / maxHP * PLAYER_BAR_ENLARGE_RATE);
	bloodBar->setPositionX(bloodBar->getPositionX() - (damage / maxHP) * bloodBar->getContentSize().width / 2 * PLAYER_BAR_ENLARGE_RATE);
}
void Player::updateExpBar(float curExp, float maxExp) {
	expBar->setScaleX(curExp / maxExp * PLAYER_BAR_ENLARGE_RATE);
	expBar->setPositionX(expBarEmpty->getPositionX() - expBarEmpty->getContentSize().width / 2 * PLAYER_BAR_ENLARGE_RATE + expBar->getContentSize().width / 2 * curExp / maxExp * PLAYER_BAR_ENLARGE_RATE);
}