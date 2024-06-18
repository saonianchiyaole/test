#include "memory"
#include "Scene/GameScene.h"

WaveManager* WaveManager::instance = nullptr;


WaveManager* WaveManager::create() {
	assert(WaveManager::instance == nullptr);

	WaveManager* pRet = WaveManager::getInstance();
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

void WaveManager::generateEnemy() {
	//暂时每波敌人的数量是线性增长的,遵循公式:EnemyAmount = a + b * wave,a、b值的设置暂无考究
	wave++;
	currentWaveEnemyAmount = 1 + 2 * (wave - 1);

	if (wave == waveSelectWeapon)
		canSelectWeapon = true;



	// 生成随机数引擎
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> edgeDist(1, 2);

	for (int i = 0; i < currentWaveEnemyAmount; i++) {

		//敌人位置随机，但总是走向Player
		Vec2 randomPositon = getRandomPosition();
		int randomType = edgeDist(gen);

		Enemy* newEnemy = EnemyFactory::addEnemy(randomPositon, randomType);

		GameScene::getInstance()->addEnemy(newEnemy);
	}
}
cocos2d::Vec2 WaveManager::getRandomPosition() {
	// 生成随机数引擎
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> edgeDist(0, 3); // 0: 上, 1: 下, 2: 左, 3: 右
	std::uniform_real_distribution<float> xDist(0, visibleSize.x);
	std::uniform_real_distribution<float> yDist(0, visibleSize.y);


	//敌人随机分布在上下左右某个边框的某个位置
	int edge = edgeDist(gen);
	switch (edge) {
		//上边框
	case 0:
		return Vec2{ xDist(gen),visibleSize.y };

		//下边框
	case 1:
		return Vec2{ xDist(gen),0 };

		//左边框
	case 2:
		return Vec2{ 0,yDist(gen) };

		//右边框
	case 3:
		return Vec2{ visibleSize.x,yDist(gen) };
	}

}

WaveManager* WaveManager::getInstance() {
	if (instance == nullptr) {
		instance = new WaveManager;
	}
	return instance;
}

void WaveManager::update(float dt) {
	checkIfNeedToGenerateEnemy();
	if (this->canSelectWeapon == true)
		callSelectWeapon();
}

void WaveManager::callSelectWeapon() {
	if (this->canSelectWeapon == false || GameScene::instance->isAddButtonOk == false)
		return;
	else
		GameScene::instance->drawSelectWeaponButton();
}

bool WaveManager::init() {

	visibleSize = Director::getInstance()->getVisibleSize();
	//初始化变量
	wave = 0;
	currentWaveEnemyAmount = 0;

	waveSelectWeapon = 2;

	canSelectWeapon = false;

	timeManager = TimeManager::getInstance();

	CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);

	return true;
}

bool WaveManager::haveEnemyHaveBeenCleaned() {
	return GameScene::instance->enemyByHashSet.size() > 0 ? false : true;
}

int WaveManager::residualEnemyAmount() {
	return GameScene::instance->enemyByHashSet.size();
}
bool WaveManager::checkIfNeedToGenerateEnemy() {
	if (haveEnemyHaveBeenCleaned()
		|| TimeManager::instance->getWaveTime() > 30.0f) {
		generateEnemy();
		//重置波次的时间
		timeManager->setWaveTime(0.0f);
		//通知GameScene让他更新波次
		GameScene::getInstance()->updateWaveNum(wave);

		return true;
	}
	return false;
}
void WaveManager::setInstanceToNullptr() {
	instance = nullptr;
}