#include "Buff/Buff_Weapon.h"
#include "Scene/GameScene.h"

Buff_Weapon::Buff_Weapon() :Buff() {
	cooldown_delta = 0;
	damage_delta = 0;
	attackTimes_delta = 0;

	// �������������
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, BUFF_NUM - 1);

	int randomNumber = dis(gen);
	//randomNumber = 0;
	switch (randomNumber) {
		//�µ�buff������Ӽ���
	case 0:
		cooldown_delta = 0.1;
		tips = u8"�����������" + std::to_string(int(cooldown_delta * 100)) + "%";
		path= "Buff/cooldown_delta.png";
		break;
	case 1:
		damage_delta = 0.2f;
		tips = u8"��������������" + std::to_string(int(damage_delta * 100)) + "%";
		path = "Buff/damage_delta.png";
		break;
	case 2:
		attackTimes_delta = 1;
		tips = u8"��������+" + std::to_string(int(attackTimes_delta));
		path = "Buff/attackTimes_delta.png";
		break;
	}

}

void Buff_Weapon::addBuff() {
	if (GameScene::getInstance()->player->weaponsPlayerOwn.size() == 0) {
		_lg("��������");
		return;
	}
	// �������������
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> edgeDist(0, GameScene::getInstance()->player->weaponsPlayerOwn.size() - 1);

	auto weapon = GameScene::getInstance()->player->weaponsPlayerOwn[edgeDist(gen)];
	weapon->cooldown *= (1 - cooldown_delta);
	weapon->damage *= (1 + damage_delta);
	weapon->attackTimes += attackTimes_delta;
}
