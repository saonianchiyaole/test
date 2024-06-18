#include"Character/Boss.h"
#include"Scene/GameScene.h"
bool Boss::init(Vec2 position) {
	if (!Enemy::init(position)) {
		return false;
	}
	// ��ʼ��Boss���ض�����
	bonus_exp = 1000;
	isMeleeMode = true; // Ĭ��ģʽΪ��սģʽ
	lastModeChangeTime = 0.0f; // ��ʼ����һ��ģʽ�л�ʱ��
	modeChangeTimer = 0.0f;
	lastAttackTime = 0.0f; // ��ʼ����һ�ι���ʱ��
	if (isMeleeMode) {
		attack_range = 50.0f;
		attackInterval = 0.3f;
	}
	else {
		attackInterval = 6.0f;
		attack_range = 300.0f;
		interval = 0.5f;
	}
	return true;
}

void Boss::update(float delta) {
	// ��ȡ��ǰʱ��
	float currentTime = Director::getInstance()->getTotalFrames() / 60.0f;
	// ÿ10���л�һ��ģʽ
	modeChangeTimer += delta;
	if (modeChangeTimer >= 10.0f) {
		changeMode();
		modeChangeTimer = 0.0f;
	}
	// ���ݵ�ǰģʽ�����ƶ��͹���

	if (isMeleeMode) {
		move();
		float distanceToPlayer = this->getPosition().distance(GameScene::getInstance()->player->getPosition());
		if (distanceToPlayer <= attack_range && (currentTime - lastAttackTime) >= attackInterval) {
			attack();
			lastAttackTime = currentTime; // ������һ�ι���ʱ��
		}
	}
	else {
		move();
		if ((currentTime - lastAttackTime) >= attackInterval) {
			attack();
			lastAttackTime = currentTime; // ������һ�ι���ʱ��
		}
	}

}

void Boss::changeMode() {
	isMeleeMode = !isMeleeMode;
}

void Boss::move() {
	if (isMeleeMode) {
		// ��սģʽ���ƶ��߼�
		auto playerPosition = GameScene::getInstance()->player->getPosition();
		auto enemyPosition = this->getPosition();

		// ������������
		Vec2 direction = playerPosition - enemyPosition;
		direction.normalize();

		// �����ƶ�������
		Vec2 moveStep = direction * interval * 2; // �ٶ�ϵ�������Ը�����Ҫ����

		// ���µ���λ��
		this->setPosition(enemyPosition + moveStep);
	}
	else {
		// Զ��ģʽ���ƶ��߼������ɵ����������ƶ�
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
}

void Boss::attack() {
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
