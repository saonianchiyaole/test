#include "Manager/EnemyFactory.h"
Enemy* EnemyFactory::addEnemy(std::string enemyType, Vec2 position) {
	return nullptr;
}
Enemy* EnemyFactory::addEnemy(Vec2 position, int enemyType) {
	Enemy* pRet = nullptr;
	std::string type;

	switch (enemyType) {
	case 1:
		type = "EnemyAnimation/batMove";
		break;
	case 2:
		type = "EnemyAnimation/wizardMove";
		break;
	default:
		pRet = new(std::nothrow) Enemy();
		break;
	}
	pRet = Enemy::create(position, type);

	if (pRet) {
		//pRet->autorelease();
		return pRet;
	}
	else {
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}