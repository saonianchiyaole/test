#include "Sword.h"
//��ʼ����������Դ
std::string Sword::pictureResourcePath = "WeaponResource/SwordQi.png";
Sword* Sword::instance = nullptr;
float Sword::swordQiMaxTimeInScene = 0.5f;

Sword::Sword() {
	//pictureResourcePath = "WeaponResource/arrow.png";
	if (checkTextureCache(Sword::pictureResourcePath)) {
		// ��������
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

	//���bug������ԭ���� ��Ҫ��scheduleUpdate
	CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);

	return true;
}
void Sword::update(float delta) {
	//���㹥�����
	deltaTime += delta;
}
void Sword::attack() {

	//���㹥�����
	if (deltaTime < cooldown)
		return;
	else
		deltaTime = 0;

	// ���õ�ǰ����Ľ�����
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

	//�������
	this->initWithFile(Sword::pictureResourcePath);

	//������������
	initPhysics();
	//�����ײ���
	addContactListener();

	//��֤��һ��������Զ����ɫǰ��
	translateAndScale(GameScene::getInstance()->player->getScaleX());

	//�����ٶ�
	float fadeSpeed = 0.05f;

	// �������붯��
	auto fadeIn = FadeIn::create(fadeSpeed);

	// ������������ʱ��
	auto delay = DelayTime::create(Sword::swordQiMaxTimeInScene);

	// ������������
	auto fadeOut = FadeOut::create(fadeSpeed);
	//�Ƴ�
	auto move = CallFunc::create([this]() {
		this->removeFromParent();
		});
	// ����һ��˳����
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
	// ����������
	auto physicsBody = PhysicsBody::createBox(this->getContentSize()); // �����ͷ���ֵ���ײ���СΪ10x10
	physicsBody->setGravityEnable(false);
	physicsBody->setDynamic(true); // ����Ϊ��̬������
	physicsBody->setCategoryBitmask(CategoryBitMask::ATTACK); // ���÷�������
	physicsBody->setCollisionBitmask(0x00000000); // ������ײ����
	physicsBody->setContactTestBitmask(CategoryBitMask::ENEMY); // ���ýӴ���������
	this->setPhysicsBody(physicsBody);
}

bool SwordQi::onContactBegin(PhysicsContact& contact) {
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


	// ���˵��� SwordQi �޹ص���ײ
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
		// ���û�� SwordQi ������ײ�����������ײ
		return false;
	}


	//���崦����ײ�Ĳ���
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
	//�����ײ������
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(SwordQi::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
}