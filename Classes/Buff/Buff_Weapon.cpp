#include "Buff/Buff_Weapon.h"
#include "Scene/GameScene.h"

Buff_Weapon::Buff_Weapon() :Buff() {
	cooldown_delta = 0;
	damage_delta = 0;
	attackTimes_delta = 0;

	// 生成随机数引擎
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, BUFF_NUM - 1);

	int randomNumber = dis(gen);
	//randomNumber = 0;
	switch (randomNumber) {
		//新的buff在这里加即可
	case 0:
		cooldown_delta = 0.1;
		tips = u8"攻击间隔减少" + std::to_string(int(cooldown_delta * 100)) + "%";
		path= "Buff/cooldown_delta.png";
		break;
	case 1:
		damage_delta = 0.2f;
		tips = u8"武器攻击力提升" + std::to_string(int(damage_delta * 100)) + "%";
		path = "Buff/damage_delta.png";
		break;
	case 2:
		attackTimes_delta = 1;
		tips = u8"攻击次数+" + std::to_string(int(attackTimes_delta));
		path = "Buff/attackTimes_delta.png";
		break;
	}

}

void Buff_Weapon::addBuff() {
	if (GameScene::getInstance()->player->weaponsPlayerOwn.size() == 0) {
		_lg("无武器！");
		return;
	}
	// 生成随机数引擎
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> edgeDist(0, GameScene::getInstance()->player->weaponsPlayerOwn.size() - 1);

	auto weapon = GameScene::getInstance()->player->weaponsPlayerOwn[edgeDist(gen)];
	weapon->cooldown *= (1 - cooldown_delta);
	weapon->damage *= (1 + damage_delta);
	weapon->attackTimes += attackTimes_delta;
}
