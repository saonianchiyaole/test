#ifndef __MELEE_ENEMY_H__

#define __MELEE_ENEMY_H__


#include"Character/Enemy.h"
class MeleeEnemy :public Enemy {
public:
	float lastAttackTime;//��¼�������ʱ��
	bool init(Vec2 position) override;
	void update(float delta) override;
	void move(); 
	void attack();
};


#endif // !__MELEE_ENEMY_H__
