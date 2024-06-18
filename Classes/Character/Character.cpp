#include "Character.h"

Character::Character() {

	isMagicAttack = false;

	m_isrunning = false;
	m_isdead = false;
	m_isAttack = false;
	m_ishurt = false;
	m_max_life = maxLife;
	m_life = m_max_life;
	m_isAlive = true;
	alreadyAttack = false;
	attackEnd = false;
	curHP = maxLife;
}

Character::~Character() {

}

Character* Character::create(Vec2 position) {
	Character* pRet = new(std::nothrow) Character();
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

bool Character::init(Vec2 position) {
	if (!Sprite::init()) {
		return false;
	}

	ArmatureDataManager::getInstance()->addArmatureFileInfo("Chapter06/AnimationScene/animation/Character/Character.ExportJson");
	m_armature = Armature::create("Character");
	if (m_armature == NULL) {
		CCLOG("Character load error!");
		return false;
	}
	m_armature->setPosition(Vec2::ZERO);
	m_armature->getAnimation()->play("loading");
	m_armature->getAnimation()->setFrameEventCallFunc(CC_CALLBACK_0(Character::onFrameEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
	this->addChild(m_armature);
	this->setPosition(position);

	//this->scheduleUpdate();

	return true;
}

void Character::initPhysicsBody() {
	// 创建物理体
	Size boundingBox = this->getContentSize();

	auto physicsBody = PhysicsBody::createBox(boundingBox); // 假设箭头部分的碰撞框大小为10x10
	physicsBody->setGravityEnable(false);
	physicsBody->setDynamic(true); // 设置为动态物理体
	physicsBody->setCategoryBitmask(0x01); // 设置分类掩码
	physicsBody->setCollisionBitmask(0x00000000); // 设置碰撞掩码
	physicsBody->setContactTestBitmask(0x01); // 设置接触测试掩码
	this->setPhysicsBody(physicsBody);
}
Animation* Character::createWalkAnimation(const std::string& prefixName, int start, int end, float delay) {
	Vector<SpriteFrame*> animFrames;
	for (int i = start; i <= end; ++i) {
		std::string frameName = prefixName + std::to_string(i) + ".png";

		// 使用 Image 类来加载图片文件
		Image img;
		if (img.initWithImageFile(frameName)) {
			auto frame = SpriteFrame::create(frameName, Rect(0, 0, img.getWidth(), img.getHeight())); // 假设每个图片的大小是100x100
			if (frame) {
				animFrames.pushBack(frame);
			}
		}
	}
	return Animation::createWithSpriteFrames(animFrames, delay);
}

void Character::createWalkAction(const std::string& prefixName, int start, int end, float delay) {
	auto walkAnimation = createWalkAnimation(prefixName, start, end, delay);
	auto animate = Animate::create(walkAnimation);
	moveAction = RepeatForever::create(animate);
	moveAction->setTag(PLAYER_MOVE_TAG);
	moveAction->retain(); // 保持引用以便之后使用
	//this->runAction(moveAction);
}

void Character::hurt(int damage) {
	if (!m_isAlive)
		false;
	curHP -= damage;
	if (curHP < 0) {
		m_isAlive = false;
	}
}


void Character::initBloodBar() {
	Size boundingBox = this->getContentSize();
	bloodBarEmpty = Sprite::create(PATH_OF_BLOOD_BAR_EMPTY);
	bloodBarEmpty->setPosition(Vec2{ 0,boundingBox.height + bloodBarEmpty->getContentSize().height });

	bloodBar = Sprite::create(PATH_OF_BLOOD_BAR);
	bloodBar->setPosition(Vec2{ 0,boundingBox.height + bloodBarEmpty->getContentSize().height });

	bloodBar->setTag(BLOOD_BAR_TAG);
	bloodBarEmpty->setTag(BLOOD_BAR_EMPTY_TAG);

	this->addChild(bloodBarEmpty, 3);
	this->addChild(bloodBar, 4);
}
void Character::updateBloodBar(float damage) {
	if (curHP <= 0) {
		bloodBar->setScaleX(0);
	}
	bloodBar->setScaleX(curHP / maxHP);
	bloodBar->setPositionX(bloodBar->getPositionX() - (damage / maxHP) * bloodBar->getContentSize().width / 2);
}
void Character::updateBloodBar() {
	bloodBar->setScaleX(PLAYER_BAR_ENLARGE_RATE * curHP / maxHP);
	bloodBar->setPositionX(bloodBarEmpty->getPositionX() - (bloodBar->getContentSize().width * (1 - curHP / maxHP) / 2) * PLAYER_BAR_ENLARGE_RATE);// 
}
void Character::removeBloodBar() {
	this->removeChildByTag(BLOOD_BAR_EMPTY_TAG);
	this->removeChildByTag(BLOOD_BAR_TAG);
}

void Character::dead() {
	this->removeFromParent();
}


void Character::removeExpBar() {
	this->removeChildByTag(GAME_SCENE_EXP_BAR_EMPTY_TAG);
	this->removeChildByTag(GAME_SCENE_EXP_BAR_TAG);
}

void Character::onFrameEvent(cocostudio::Bone* bone, const std::string& evt, int originFrameIndex, int currentFrameIndex) {
	//if (strcmp(evt.c_str(), "attack_end") == 0)//攻击结束
	//{
	//	m_characterState = LOADING;
	//	m_armature->getAnimation()->play("loading");
	//	m_isAttack = false;
	//	alreadyAttack = false;
	//	attackEnd = true;
	//}
	//if (strcmp(evt.c_str(), "attack") == 0)//触发攻击音效
	//{
	//	if (dynamic_cast<Hero*>(this)) {
	//		playSound(PATH_OF_SOUND_OF_HERO_ATTACK);
	//	}
	//	else {
	//		playSound(PATH_OF_SOUND_OF_ENEMY_ATTACK);
	//	}
	//}
	//if (strcmp(evt.c_str(), "smitten_end") == 0)//被控制结束
	//{
	//	m_characterState = LOADING;
	//	m_armature->getAnimation()->play("loading");
	//}
	//if (strcmp(evt.c_str(), "revive_end") == 0)//复活动作结束
	//{
	//	m_characterState = LOADING;
	//	m_armature->getAnimation()->play("loading");
	//}
	//if (strcmp(evt.c_str(), "defend_begin") == 0)//触发防御音效
	//{
	//	playSound(PATH_OF_SOUND_OF_DEFEND);
	//}
	//if (strcmp(evt.c_str(), "run_begin") == 0)//触发跑步音效
	//{
	//	if (dynamic_cast<Hero*>(this)) {
	//		playSound(PATH_OF_SOUND_OF_HERO_RUNNING);
	//	}
	//	else {
	//		playSound(PATH_OF_SOUND_OF_ENEMY_RUNNING);
	//	}
	//}
}

//void Character::play(SignalState state) {
//	//if (state == SMITTEN) // 控制被击中时颤抖动画只播放一次
//	//{
//	//	m_ishurt = true;
//	//}
//	m_signalState = state;
//	if (state == REVIVE && m_characterState == DEATH) {
//		revive();
//		m_characterState = LOADING;
//		m_armature->getAnimation()->play("revive");
//	}
//}

//void Character::hurt(float val) {
//	// 根据基础伤害造成随机伤害
//	showBloodTips(val);
//	//this->play(SMITTEN);
//	if ((m_characterState != DEATH) && (m_characterState != BEING_CONTROLLED) && (m_characterState != DEFENDING)) {
//		m_characterState = BEING_CONTROLLED;
//		m_armature->getAnimation()->play("smitten");
//	}
//}

// 显示扣血 
//void Character::showBloodTips(int s) {
//	int hitCount = 1;
//	int hitRand = getRand(10.0f);
//	if (hitRand > 3 && hitRand < 8) {
//		hitCount = 2;
//	}
//	else if (hitRand > 7) {
//		hitCount = 3;
//	}
//
//	for (int i = 0; i < hitCount; i++) {
//		int hurt_blood = s + getRand(8.0f);
//		if (m_characterState == DEFENDING) {
//			hurt_blood /= 10;
//		}
//		setLife(max(m_life - hurt_blood, 0)); // 扣血
//		auto label = Label::createWithBMFont("fonts/futura-48.fnt", StringUtils::format("-%d", hurt_blood));
//		if (dynamic_cast<Hero*>(this))
//			label->setColor(Color3B::RED);
//		this->addChild(label, 5);
//		label->setPosition(Vec2(0, 0) + Vec2(20 + rand() % 80, 10 + rand() % 80));
//		label->runAction(Sequence::create(
//			MoveBy::create(0.7f, Vec2(0, 30)),
//			CallFunc::create(CC_CALLBACK_0(Character::flyend, this, label)),
//			NULL
//		));
//	}
//}

/*void Character::flyend(Label label) {
	label->setVisible(false);
	label->removeFromParent();
}*/
