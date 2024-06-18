#include "Weapon/Bow.h"
//��ʼ����������Դ
std::string Bow::pictureResourcePath = "WeaponResource/arrow.png";
Bow* Bow::instance = nullptr;
float Bow::maxDistance = 0.0f;

Bow::Bow() {
	//pictureResourcePath = "WeaponResource/arrow.png";
	if (checkTextureCache(Bow::pictureResourcePath)) {
		// ��������
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

	//���bug������ԭ���� ��Ҫ��scheduleUpdate
	CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);

	return true;
}
void Bow::update(float delta) {
	//���㹥�����
	deltaTime += delta;
}
void Bow::attack() {

	//���㹥�����
	if (deltaTime < cooldown)
		return;
	else
		deltaTime = 0;

	// ���õ�ǰ����ļ���
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

	//�������
	this->initWithFile(Bow::pictureResourcePath);

	//������������
	initPhysics();
	//�����ײ���
	addContactListener();

	//��ʸ�˶���ؼ���
	translateAndRotate();



	//this->setTexture(Director::getInstance()->getTextureCache()->textureForKey(Bow::pictureResourcePath));
	maxDistance = Bow::getInstance()->maxDistance;

	//��ǰ��ʸ�ı�ţ����ڴ���
	int thisArrowSerialNumber;


	CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);


	return true;
}
void Arrow::translateAndRotate() {
	//��ʸ�˶���ؼ��㣬��ò�Ҫ��
	Vec2 direction = GameScene::getInstance()->mousePosition;
	startPosition = GameScene::getInstance()->player->getPosition();
	this->setPosition(startPosition);

	Vec2 delta = (direction - startPosition);
	delta = delta.getNormalized();
	dx = delta.x;
	dy = delta.y;
	speed = Bow::getInstance()->speed;

	// ���㻡��
	float angleRadians = atan2(delta.x, delta.y);

	// ������ת��Ϊ�Ƕ�
	float angleDegrees = CC_RADIANS_TO_DEGREES(angleRadians);

	this->setRotation(angleDegrees - 90);
}
void Arrow::initPhysics() {
	// ����������
	auto physicsBody = PhysicsBody::createBox(this->getContentSize()); // �����ͷ���ֵ���ײ���СΪ10x10
	physicsBody->setGravityEnable(false);
	physicsBody->setDynamic(true); // ����Ϊ��̬������
	physicsBody->setCategoryBitmask(CategoryBitMask::ATTACK); // ���÷�������
	physicsBody->setCollisionBitmask(0x00000000); // ������ײ����
	physicsBody->setContactTestBitmask(CategoryBitMask::ENEMY); // ���ýӴ���������
	this->setPhysicsBody(physicsBody);
}

bool Arrow::onContactBegin(PhysicsContact& contact) {
	// ������ײ�¼�
	auto shapeA = contact.getShapeA();
	auto shapeB = contact.getShapeB();
	auto bodyA = shapeA->getBody();
	auto bodyB = shapeB->getBody();
	//��ö���Ļ���ָ��
	auto nodeA = bodyA->getNode();
	auto nodeB = bodyB->getNode();
	//�������
	int categoryA = bodyA->getCategoryBitmask();
	int categoryB = bodyB->getCategoryBitmask();
	//���崦����ײ�Ĳ���
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
	//��ʸ�˶�
	this->setPosition(this->getPositionX() + dx * speed * dt, this->getPositionY() + dy * speed * dt);
	if (this->getPosition().distance(startPosition) >= maxDistance) {
		this->removeFromParent();
	}
}

void Arrow::addContactListener() {
	//�����ײ������
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(Arrow::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
}