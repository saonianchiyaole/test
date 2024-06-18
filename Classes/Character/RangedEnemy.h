#ifndef __RANGED_ENEMY_H__

#define __RANGED_ENEMY_H__




#include"Character/Enemy.h"
class RangedEnemy :public Enemy {
public:
	float lastAttackTime;//��¼�������ʱ��
	float moveChangeDirectionInterval; // �ƶ�����仯���
	float lastMoveChangeTime; // ��һ���ƶ�����仯ʱ��
	Vec2 moveDirection; // ��ǰ�ƶ�����
	bool init(Vec2 position) override;
	void update(float delta) override;
	void move(); // ���ڿ��Ƶ�����ԭ�ع��ɵ����������ƶ�
	void attack();
};


#endif