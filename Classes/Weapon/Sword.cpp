#include "Sword.h"
//初始化弓箭的资源
std::string Sword::pictureResourcePath = "WeaponResource/SwordQi.png";
Sword* Sword::instance = nullptr;
float Sword::swordQiMaxTimeInScene = 0.5f;

Sword::Sword() {
	//pictureResourcePath = "WeaponResource/arrow.png";
	if (checkTextureCache(Sword::pictureResourcePath)) {
		// 加载纹理
		Director::getInstance()->getTextureCache()->addImage(pictureResourcePath);
	}
}
Sword* Sword::create() {
	Sword* pRet = new(std::nothrow) Sword();
	if (pRet && pRet->init()) {
		//pRet->autorelease();
		return pRet;
	}
	else {
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}
Sword::~Sword() {

}
bool Sword::init() {
	if (!Sprite::init())
	{
		return false;
	}

	damage = 10;
	deltaTime = 1.0f;
	attackTimes = 1;
	speed = 1.0f;
	cooldown = 1.2f;
	swordQiMaxTimeInScene = 0.05f;
	attackTimesInOneAttack = 0;
	attackInterval = 0.1f;
	pictureResourcePath = "WeaponResource/SwordQi.png";

	//解决bug，这里原因不明 不要用scheduleUpdate
	CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);

	return true;
}
void Sword::update(float delta) {
	//计算攻击间隔
	deltaTime += delta;
}
void Sword::attack() {

	//计算攻击间隔
	if (deltaTime < cooldown)
		return;
	else
		deltaTime = 0;

	// 重置当前射出的剑气数
	attackTimesInOneAttack = 0;
	cocos2d::Director::getInstance()->getScheduler()->schedule(
		CC_SCHEDULE_SELECTOR(Sword::shootSwordQis), this, attackInterval, false);

}
void Sword::shootSwordQis(float dt) {
	if (attackTimesInOneAttack < attackTimes) {
		shootSwordQi();
		attackTimesInOneAttack++;
	}
	else {
		unschedule(CC_SCHEDULE_SELECTOR(Sword::shootSwordQis));
	}
}
void Sword::shootSwordQi() {
	auto swordQi = SwordQi::create();
	GameScene::getInstance()->addChild(swordQi, ATTACK_LAYER);
}

Sword* Sword::getInstance() {
	if (instance == nullptr) {
		instance = Sword::create();
	}
	return instance;
}

SwordQi* SwordQi::create() {
	SwordQi* pRet = new(std::nothrow) SwordQi();
	if (pRet && pRet->init()) {
		pRet->autorelease();
		return pRet;
	}
	else {
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

bool SwordQi::init() {

	//添加纹理
	this->initWithFile(Sword::pictureResourcePath);

	//适用物理引擎
	initPhysics();
	//添加碰撞检测
	addContactListener();

	//保证第一发剑气永远朝角色前方
	translateAndScale(GameScene::getInstance()->player->getScaleX());

	//攻击速度
	float fadeSpeed = 0.05f;

	// 创建渐入动作
	auto fadeIn = FadeIn::create(fadeSpeed);

	// 创建剑气存在时间
	auto delay = DelayTime::create(Sword::swordQiMaxTimeInScene);

	// 创建渐出动作
	auto fadeOut = FadeOut::create(fadeSpeed);
	//移除
	auto move = CallFunc::create([this]() {
		this->removeFromParent();
		});
	// 创建一个顺序动作
	auto sequence = Sequence::create(fadeIn, delay, fadeOut, move, nullptr);



	this->runAction(sequence);
	//CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);


	return true;
}
void SwordQi::translateAndScale(int direction) {
	int finalDirection = direction * ((Sword::getInstance()->attackTimesInOneAttack % 2) ? -1 : 1);
	this->setScale(finalDirection);
	auto startPosition = GameScene::getInstance()->player->getPosition() + Vec2(30 + 120 * finalDirection, 20 + 10 * finalDirection);
	this->setPosition(startPosition);
}
void SwordQi::initPhysics() {
	// 创建物理体
	auto physicsBody = PhysicsBody::createBox(this->getContentSize()); // 假设箭头部分的碰撞框大小为10x10
	physicsBody->setGravityEnable(false);
	physicsBody->setDynamic(true); // 设置为动态物理体
	physicsBody->setCategoryBitmask(CategoryBitMask::ATTACK); // 设置分类掩码
	physicsBody->setCollisionBitmask(0x00000000); // 设置碰撞掩码
	physicsBody->setContactTestBitmask(CategoryBitMask::ENEMY); // 设置接触测试掩码
	this->setPhysicsBody(physicsBody);
}

bool SwordQi::onContactBegin(PhysicsContact& contact) {
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


	// 过滤掉与 SwordQi 无关的碰撞
	SwordQi* swordQi = nullptr;
	Node* otherNode = nullptr;
	if (dynamic_cast<SwordQi*>(nodeA)) {
		swordQi = dynamic_cast<SwordQi*>(nodeA);
		otherNode = nodeB;
	}
	else if (dynamic_cast<SwordQi*>(nodeB)) {
		swordQi = dynamic_cast<SwordQi*>(nodeB);
		otherNode = nodeA;
	}

	if (!swordQi) {
		// 如果没有 SwordQi 参与碰撞，忽略这次碰撞
		return false;
	}


	//具体处理碰撞的部分
	if ((categoryA == CategoryBitMask::ATTACK && categoryB == CategoryBitMask::ENEMY) ||
		(categoryA == CategoryBitMask::ENEMY && categoryB == CategoryBitMask::ATTACK)) {
		// Arrow collided with another object
		if (nodeA && categoryA == CategoryBitMask::ATTACK) {
			auto enemy = dynamic_cast<Enemy*>(nodeB);
			if (enemy && enemy->m_isAlive) {
				enemy->hurt(Sword::getInstance()->damage+GameScene::getInstance()->player->attackCapacity);
			}
		}
		else if (nodeB && categoryB == CategoryBitMask::ATTACK) {
			auto enemy = dynamic_cast<Enemy*>(nodeA);
			if (enemy && enemy->m_isAlive) {
				enemy->hurt(Sword::getInstance()->damage + GameScene::getInstance()->player->attackCapacity);
			}
		}
	}

	return true;
}

void SwordQi::addContactListener() {
	//添加碰撞侦听器
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(SwordQi::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
}