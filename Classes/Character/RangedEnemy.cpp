#include "Character/RangedEnemy.h"
#include "Scene/GameScene.h"
// Զ�̵���ʵ��
bool RangedEnemy::init(Vec2 position) {
	if (!Enemy::init(position)) {
		return false;
	}
	// ��ʼ��Զ�̵��˵��ض�����
	bonus_exp = 50.0f;
	attack_range = 200.0f; // ʾ��ֵ��������Ҫ����
	interval = 1.0f; // �ƶ��������ʾ�ƶ����ٶ�
	attackInterval = 5.0f; // ʾ��ֵ�����ι���֮��ļ��ʱ��Ϊ2��
	lastAttackTime = 0.0f; // ��ʼ����һ�ι���ʱ��
	moveChangeDirectionInterval = 1.0f; // ÿ��1��ı�һ���ƶ�����
	lastMoveChangeTime = 0.0f; // ��ʼ����һ���ƶ�����仯ʱ��
	moveDirection = Vec2::ZERO; // ��ʼ���ƶ�����
	return true;
}

void RangedEnemy::update(float delta) {
	// ��ȡ��ǰʱ��
	float currentTime = Director::getInstance()->getTotalFrames() / 60.0f; // �򵥵�֡��ʱ��������ÿ��60֡

	// �����ƶ��߼�
	move();

	// �жϹ�������Ƿ�����
	if ((currentTime - lastAttackTime) >= attackInterval) {
		attack();
		lastAttackTime = currentTime; // ������һ�ι���ʱ��
	}
}

void RangedEnemy::move() {
	float currentTime = Director::getInstance()->getTotalFrames() / 60.0f; // �򵥵�֡��ʱ��������ÿ��60֡

	// ����Ƿ���Ҫ�ı��ƶ�����
	if ((currentTime - lastMoveChangeTime) >= moveChangeDirectionInterval) {
		// ���ѡ���µ��ƶ�����
		int direction = rand() % 4;
		switch (direction) {
		case 0:
			moveDirection = Vec2(0, 1); // ����
			break;
		case 1:
			moveDirection = Vec2(0, -1); // ����
			break;
		case 2:
			moveDirection = Vec2(-1, 0); // ����
			break;
		case 3:
			moveDirection = Vec2(1, 0); // ����
			break;
		}
		lastMoveChangeTime = currentTime; // ������һ���ƶ�����仯ʱ��
	}
	// ���µ���λ��
	Vec2 moveStep = moveDirection * interval * 1; // �ٶ�ϵ�������Ը�����Ҫ����
	this->setPosition(this->getPosition() + moveStep);
}

void RangedEnemy::attack() {
	// ��ȡ��ҵ�ʵ��
	auto player = GameScene::getInstance()->player;
	// ȷ����ҵ�ǰ��λ��
	auto playerPosition = player->getPosition();

	// �ڴ˴�ִ�й���������Ч��
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
