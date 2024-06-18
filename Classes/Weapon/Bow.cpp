#include "Weapon/Bow.h"
//初始化弓箭的资源
std::string Bow::pictureResourcePath = "WeaponResource/arrow.png";
Bow* Bow::instance = nullptr;
float Bow::maxDistance = 0.0f;

Bow::Bow() {
	//pictureResourcePath = "WeaponResource/arrow.png";
	if (checkTextureCache(Bow::pictureResourcePath)) {
		// 加载纹理
		Director::getInstance()->getTextureCache()->addImage(pictureResourcePath);
	}
}
Bow* Bow::create() {
	Bow* pRet = new(std::nothrow) Bow();
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
bool Bow::init() {
	if (!Sprite::init())
	{
		return false;
	}

	damage = 10;
	deltaTime = 0.0f;
	attackTimes = 1;
	speed = 600.0f;
	cooldown = 1.5f;
	attackInterval = 0.1f;
	maxDistance = 1000.f;
	attackTimesInOneAttack = 0;
	pictureResourcePath = "WeaponResource/arrow.png";

	//解决bug，这里原因不明 不要用scheduleUpdate
	CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);

	return true;
}
void Bow::update(float delta) {
	//计算攻击间隔
	deltaTime += delta;
}
void Bow::attack() {

	//计算攻击间隔
	if (deltaTime < cooldown)
		return;
	else
		deltaTime = 0;

	// 重置当前射出的箭数
	attackTimesInOneAttack = 0;
	cocos2d::Director::getInstance()->getScheduler()->schedule(
		CC_SCHEDULE_SELECTOR(Bow::shootMultipleArrows), this, attackInterval, false);

}
void Bow::shootMultipleArrows(float dt) {
	if (attackTimesInOneAttack < attackTimes) {
		shootArrow();
		attackTimesInOneAttack++;
	}
	else {
		unschedule(CC_SCHEDULE_SELECTOR(Bow::shootMultipleArrows));
	}
}
Bow::~Bow() {

}
void Bow::shootArrow() {
	auto arrow = Arrow::create();
	GameScene::getInstance()->addChild(arrow, ATTACK_LAYER);
}

Bow* Bow::getInstance() {
	if (instance == nullptr) {
		instance = Bow::create();
	}
	return instance;
}

Arrow* Arrow::create() {
	Arrow* pRet = new(std::nothrow) Arrow();
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

bool Arrow::init() {

	//添加纹理
	this->initWithFile(Bow::pictureResourcePath);

	//适用物理引擎
	initPhysics();
	//添加碰撞检测
	addContactListener();

	//箭矢运动相关计算
	translateAndRotate();



	//this->setTexture(Director::getInstance()->getTextureCache()->textureForKey(Bow::pictureResourcePath));
	maxDistance = Bow::getInstance()->maxDistance;

	//当前箭矢的编号，用于处理
	int thisArrowSerialNumber;


	CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);


	return true;
}
void Arrow::translateAndRotate() {
	//箭矢运动相关计算，最好不要改
	Vec2 direction = GameScene::getInstance()->mousePosition;
	startPosition = GameScene::getInstance()->player->getPosition();
	this->setPosition(startPosition);

	Vec2 delta = (direction - startPosition);
	delta = delta.getNormalized();
	dx = delta.x;
	dy = delta.y;
	speed = Bow::getInstance()->speed;

	// 计算弧度
	float angleRadians = atan2(delta.x, delta.y);

	// 将弧度转换为角度
	float angleDegrees = CC_RADIANS_TO_DEGREES(angleRadians);

	this->setRotation(angleDegrees - 90);
}
void Arrow::initPhysics() {
	// 创建物理体
	auto physicsBody = PhysicsBody::createBox(this->getContentSize()); // 假设箭头部分的碰撞框大小为10x10
	physicsBody->setGravityEnable(false);
	physicsBody->setDynamic(true); // 设置为动态物理体
	physicsBody->setCategoryBitmask(CategoryBitMask::ATTACK); // 设置分类掩码
	physicsBody->setCollisionBitmask(0x00000000); // 设置碰撞掩码
	physicsBody->setContactTestBitmask(CategoryBitMask::ENEMY); // 设置接触测试掩码
	this->setPhysicsBody(physicsBody);
}

bool Arrow::onContactBegin(PhysicsContact& contact) {
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
	if ((categoryA == CategoryBitMask::ATTACK && categoryB == CategoryBitMask::ENEMY) ||
		(categoryA == CategoryBitMask::ENEMY && categoryB == CategoryBitMask::ATTACK)) {
		// Arrow collided with another object
		if (nodeA && categoryA == CategoryBitMask::ATTACK) {
			auto enemy = dynamic_cast<Enemy*>(nodeB);
			if (enemy && enemy->m_isAlive) {
				enemy->hurt(Bow::getInstance()->damage + GameScene::getInstance()->player->attackCapacity);
			}
			nodeA->removeFromParent();
		}
		else if (nodeB && categoryB == CategoryBitMask::ATTACK) {
			auto enemy = dynamic_cast<Enemy*>(nodeA);
			if (enemy && enemy->m_isAlive) {
				enemy->hurt(Bow::getInstance()->damage + GameScene::getInstance()->player->attackCapacity);
			}
			nodeB->removeFromParent();
		}
	}

	return true;
}

void Arrow::update(float dt) {
	//箭矢运动
	this->setPosition(this->getPositionX() + dx * speed * dt, this->getPositionY() + dy * speed * dt);
	if (this->getPosition().distance(startPosition) >= maxDistance) {
		this->removeFromParent();
	}
}

void Arrow::addContactListener() {
	//添加碰撞侦听器
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(Arrow::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
}