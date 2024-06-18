#include "Character/MeleeEnemy.h"
#include "Scene/GameScene.h"
bool MeleeEnemy::init(Vec2 position) {
	if (!Enemy::init(position)) {
		return false;
	}
	// ��ʼ����ս���˵��ض�����
	bonus_exp = 50.0f;
	attack_range = 100.0f; // ʾ��ֵ��������Ҫ����
	interval = 0.1f; // �ƶ��������ʾ�ƶ����ٶ�
	attackInterval = 2.0f; // ʾ��ֵ�����ι���֮��ļ��ʱ��Ϊ2��
	lastAttackTime = 0.0f; // ��ʼ����һ�ι���ʱ��
	return true;
}

void MeleeEnemy::update(float delta) {
	// �����ƶ��͹����߼�
	move();
	// ��������ҵľ���
	float distanceToPlayer = this->getPosition().distance(GameScene::getInstance()->player->getPosition());
	// ��ȡ��ǰʱ��
	float currentTime = Director::getInstance()->getTotalFrames() / 60.0f; // �򵥵�֡��ʱ��������ÿ��60֡
	// �жϹ�������͹�������Ƿ�����
	if (distanceToPlayer <= attack_range && (currentTime - lastAttackTime) >= attackInterval) {
		attack();
		lastAttackTime = currentTime; // ������һ�ι���ʱ��
	}
}

void MeleeEnemy::move() {
	auto playerPosition = GameScene::getInstance()->player->getPosition();
	auto enemyPosition = this->getPosition();
	// ������������
	Vec2 direction = playerPosition - enemyPosition;
	direction.normalize();
	// �����ƶ�������
	Vec2 moveStep = direction * interval * 40; // �ٶ�ϵ�������Ը�����Ҫ����
	// ���µ���λ��
	this->setPosition(enemyPosition + moveStep);
}

void MeleeEnemy::attack() {
	// 
	auto player = GameScene::getInstance()->player;
	// 
	auto playerPosition = player->getPosition();

	//
	if (m_isdead == false && (m_ishurt == false))
	{
		m_isAttack = true;
		m_armature->getAnimation()->play("attack");
	}
	// ��������Ч����������ҵ�����ֵ
	float attackDamage = 10.0f; // ������Ҫ���������˺�	
	//�˴���Ҫ������ײ���
	player->hurt(attackDamage);
}