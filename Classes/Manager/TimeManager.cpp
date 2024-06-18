
#include "Manager/TimeManager.h"

TimeManager* TimeManager::instance = nullptr;

TimeManager* TimeManager::getInstance() {
	if (instance == nullptr) {
		instance = new TimeManager;
	}
	return instance;
}
TimeManager* TimeManager::create() {
	assert(TimeManager::instance == nullptr);

	TimeManager* pRet = TimeManager::getInstance();
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
void TimeManager::update(float dt) {
	timer += dt;
	if (ifInGameScene) {
		inGameTime += dt;
	}
	if (ifWaveStart) {
		waveTime += dt;
	}
}
bool TimeManager::init() {
	timer = 0.f;
	inGameTime = 0.f;
	waveTime = 0.f;
	cocos2d::CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
	return true;
}
float TimeManager::getTimer() {
	return timer;
}
float TimeManager::getInGameTime() {
	return inGameTime;
}
float TimeManager::getWaveTime() {
	return waveTime;
}
void TimeManager::setTimer(float t) {
	timer = t;
}
void TimeManager::setWaveTime(float t) {
	waveTime = t;
}
void TimeManager::setInGameTime(float t) {
	inGameTime = t;
}