#include "Character/Enemy.h"
//#include"Character/Player.h"
#include "Scene/GameScene.h"
Enemy::Enemy() :Character()
{
	bonus_exp = 100;

	//lives_num = LIVES_OF_ENEMY;
	m_isrunning = false;
	m_isdead = false;
	m_isAttack = false;
	m_ishurt = false;
	maxHP = 50;
	m_max_life = 50;

	curHP = maxHP;
	m_isAlive = true;
	attackCapacity = 10;
}

//Enemy::~Enemy()
//{
//
//}
//
Enemy* Enemy::create(Vec2 position)
{
	Enemy* pRet = new(std::nothrow) Enemy();
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
Enemy* Enemy::create(Vec2 position, std::string prefixName)
{
	Enemy* pRet = new(std::nothrow) Enemy();
	if (pRet && pRet->init(position, prefixName)) {
		pRet->autorelease();
		return pRet;
	}
	else {
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}
bool Enemy::init(Vec2 position)
{
	if (!Sprite::init())
	{
		return false;
	}

	// 暂用原来的动画
	/*ArmatureDataManager::getInstance()->addArmatureFileInfo(PATH_OF_ARMATURE_OF_ENEMY);
	m_armature = Armature::create("tauren");
	if (m_armature == NULL)
	{
		CCLOG("tauren load error!");
		return false;
	}
	m_armature->setPosition(Vec2::ZERO);
	m_armature->getAnimation()->play("loading");
	m_armature->getAnimation()->setFrameEventCallFunc(CC_CALLBACK_0(Enemy::onFrameEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
	*/
	this->setTexture("EnemyAnimation/wizardMove0.png");

	createWalkAction("EnemyAnimation/wizardMove", 0, 3, 0.27f);

	//this->addChild(m_armature);
	this->setPosition(position);
	leftOrRight = (position < GameScene::getInstance()->player->getPosition()) ? -1 : 1;
	this->setScaleX(leftOrRight);
	this->runAction(moveAction);

	//设置血条
	initBloodBar();
	this->scheduleUpdate();
	//物理引擎
	initPhysicsBody();


	return true;
}
bool Enemy::init(Vec2 position, std::string prefixName)
{
	if (!Sprite::init())
	{
		return false;
	}

	// 暂用原来的动画
	/*ArmatureDataManager::getInstance()->addArmatureFileInfo(PATH_OF_ARMATURE_OF_ENEMY);
	m_armature = Armature::create("tauren");
	if (m_armature == NULL)
	{
		CCLOG("tauren load error!");
		return false;
	}
	m_armature->setPosition(Vec2::ZERO);
	m_armature->getAnimation()->play("loading");
	m_armature->getAnimation()->setFrameEventCallFunc(CC_CALLBACK_0(Enemy::onFrameEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
	*/
	this->setTexture(prefixName + "0.png");

	createWalkAction(prefixName, 0, 3, 0.27f);

	//this->addChild(m_armature);
	this->setPosition(position);
	leftOrRight = (position < GameScene::getInstance()->player->getPosition()) ? -1 : 1;
	this->setScaleX(leftOrRight);
	this->runAction(moveAction);

	//设置血条
	initBloodBar();
	this->scheduleUpdate();
	//物理引擎
	initPhysicsBody();


	return true;
}
void Enemy::initPhysicsBody() {
	// 创建物理体
	Size boundingBox = this->getContentSize();
	auto physicsBody = PhysicsBody::createBox(boundingBox); // 假设箭头部分的碰撞框大小为10x10
	physicsBody->setGravityEnable(false);
	physicsBody->setDynamic(true); // 设置为动态物理体
	physicsBody->setCategoryBitmask(CategoryBitMask::ENEMY); // 设置分类掩码
	physicsBody->setCollisionBitmask(0x00000000); // 设置碰撞掩码
	physicsBody->setContactTestBitmask(CategoryBitMask::PLAYER | CategoryBitMask::ATTACK); // 设置接触测试掩码
	this->setPhysicsBody(physicsBody);
}
void Enemy::hurt(int damage) {
	if (!m_isAlive)
		false;
	curHP -= damage;
	//更新血条
	updateBloodBar(damage);
	if (curHP <= 0) {
		m_isAlive = false;
		this->dead();

		//给Player加经验
		GameScene::getInstance()->player->processExp(bonus_exp);
	}
}
void Enemy::dead() {
	this->getPhysicsBody()->setEnabled(false);
	removeBloodBar();
	auto callback = CallFunc::create(CC_CALLBACK_0(Enemy::onDeathAnimationFinished, this));
	this->runAction(callback);
}

void Enemy::onDeathAnimationFinished()
{

	// 将怪物节点从父节点中移除
	this->removeFromParent();
	//处理GameScene中的记录enemy的数据结构
	GameScene::getInstance()->removeEnemyFromHashSet(this);

}
void Enemy::move(float dt) {
	if (!m_isAlive)
		return;
	// 获取玩家位置
	Vec2 playerPosition = GameScene::getInstance()->player->getPosition();

	if ((this->getPosition().x < playerPosition.x ? -1 : 1) != leftOrRight) {
		if (leftOrRight == 1)
		{
			this->setScaleX(-1);
			leftOrRight = -1;
			//this->bloodBar->setScaleX(-1);
			//this->bloodBarEmpty->setScaleX(-1);
		}
		else
		{
			leftOrRight = 1;
			this->setScaleX(1);
			//this->bloodBar->setScaleX(-1);
			//this->bloodBarEmpty->setScaleX(-1);
		}
	}


	// 计算移动方向向量
	Vec2 direction = playerPosition - getPosition();
	direction.normalize();  // 归一化方向向量

	// 设置移动速度
	float speed = 100.0f;  // 假设移动速度为100像素/秒

	// 根据方向向量和速度更新位置
	Vec2 newPosition = getPosition() + direction * speed * dt;
	setPosition(newPosition);
}
void Enemy::update(float dt)
{
	if (GameScene::getInstance()->isGameOver)
		return;
	move(dt);
}
