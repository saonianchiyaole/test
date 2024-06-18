#include "Weapon/Weapon.h"

Weapon::Weapon() {

}

Weapon::~Weapon() {

}

Weapon* Weapon::create() {
	Weapon* pRet = new(std::nothrow) Weapon();
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

bool Weapon::init() {
	if (!Sprite::init()) {
		return false;
	}

	/*ArmatureDataManager::getInstance()->addArmatureFileInfo("Chapter06/AnimationScene/animation/Weapon/Weapon.ExportJson");
	m_armature = Armature::create("Weapon");
	if (m_armature == NULL) {
		CCLOG("Weapon load error!");
		return false;
	}
	m_armature->setPosition(Vec2::ZERO);
	m_armature->getAnimation()->play("loading");
	m_armature->getAnimation()->setFrameEventCallFunc(CC_CALLBACK_0(Weapon::onFrameEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
	this->addChild(m_armature);
	this->setPosition(position);*/

	//this->scheduleUpdate();

	return true;
}

void Weapon::attack() {

}
bool Weapon::checkTextureCache(const std::string& textureName) {
	Texture2D* texture = Director::getInstance()->getTextureCache()->getTextureForKey(textureName);

	if (texture) {
		CCLOG("Texture %s is already cached.", textureName.c_str());
		return false;
	}
	else {
		CCLOG("Texture %s is not cached.", textureName.c_str());
		return true;
	}
}