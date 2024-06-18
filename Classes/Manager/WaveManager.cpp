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
	//��ʱÿ�����˵�����������������,��ѭ��ʽ:EnemyAmount = a + b * wave,a��bֵ���������޿���
	wave++;
	currentWaveEnemyAmount = 1 + 2 * (wave - 1);

	if (wave == waveSelectWeapon)
		canSelectWeapon = true;



	// �������������
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> edgeDist(1, 2);

	for (int i = 0; i < currentWaveEnemyAmount; i++) {

		//����λ�����������������Player
		Vec2 randomPositon = getRandomPosition();
		int randomType = edgeDist(gen);

		Enemy* newEnemy = EnemyFactory::addEnemy(randomPositon, randomType);

		GameScene::getInstance()->addEnemy(newEnemy);
	}
}
cocos2d::Vec2 WaveManager::getRandomPosition() {
	// �������������
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> edgeDist(0, 3); // 0: ��, 1: ��, 2: ��, 3: ��
	std::uniform_real_distribution<float> xDist(0, visibleSize.x);
	std::uniform_real_distribution<float> yDist(0, visibleSize.y);


	//��������ֲ�����������ĳ���߿��ĳ��λ��
	int edge = edgeDist(gen);
	switch (edge) {
		//�ϱ߿�
	case 0:
		return Vec2{ xDist(gen),visibleSize.y };

		//�±߿�
	case 1:
		return Vec2{ xDist(gen),0 };

		//��߿�
	case 2:
		return Vec2{ 0,yDist(gen) };

		//�ұ߿�
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
	//��ʼ������
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
		//���ò��ε�ʱ��
		timeManager->setWaveTime(0.0f);
		//֪ͨGameScene�������²���
		GameScene::getInstance()->updateWaveNum(wave);

		return true;
	}
	return false;
}
void WaveManager::setInstanceToNullptr() {
	instance = nullptr;
}