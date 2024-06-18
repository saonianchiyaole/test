#include "Buff/Buff_Player.h"
#include "Scene/GameScene.h"

Buff_Player::Buff_Player() :Buff() {
	attackCapacity_delta = 0;
	criticalHitRate_delta = 0;
	defendCapacity_delta = 0;
	maxHP_delta = 0;
	curHP_delta = 0;

	// �������������
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, BUFF_NUM - 1);

	int randomNumber = dis(gen);
	//randomNumber = 4;
	switch (randomNumber) {
		//�µ�buff������Ӽ���
	case 0:
		attackCapacity_delta = 10;
		tips = u8"������+" + std::to_string(int(attackCapacity_delta));
		path = "Buff/attackCapacity_delta.png";
		break;
	case 1:
		criticalHitRate_delta = 0.08;
		tips = u8"������+" + std::to_string(int(criticalHitRate_delta * 100)) + "%";
		path = "Buff/criticalHitRate_delta.png";
		break;
	case 2:
		defendCapacity_delta = 10;
		tips = u8"������+" + std::to_string(int(defendCapacity_delta));
		path = "Buff/defendCapacity_delta.png";
		break;
	case 3:
		maxHP_delta = 0.3;
		tips = u8"�������ֵ+" + std::to_string(int(maxHP_delta * 100)) + "%";
		path = "Buff/maxHP_delta.png";
		break;
	case 4:
		curHP_delta = 5000000;
		tips = u8"Ѫ������";
		path = "Buff/curHP_delta.png";
		break;
	}

}

void Buff_Player::addBuff() {
	auto player = GameScene::getInstance()->player;
	player->attackCapacity += attackCapacity_delta;
	player->criticalHitRate = max(player->criticalHitRate + criticalHitRate_delta, 1.0f);
	player->defendCapacity += defendCapacity_delta;
	player->maxHP += maxHP_delta * player->maxHP;
	player->curHP = std::min(player->curHP + curHP_delta, player->maxHP);
	player->Character::updateBloodBar();
}
