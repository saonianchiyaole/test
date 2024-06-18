#ifndef __BOSS_H__

#define __BOSS_H__


#include"Character/Enemy.h"
class Boss :public Enemy {
public:
	float modeChangeTimer; // ģʽ�л���ʱ��
	float lastModeChangeTime; // ��һ��ģʽ�л�ʱ��
	float lastAttackTime; // ��һ�ι���ʱ��
	float lastMoveChangeTime;
	float moveChangeDirectionInterval; // �ƶ�����仯���
	Vec2 moveDirection; // ��ǰ�ƶ�����
	bool isMeleeMode;
	bool init(Vec2 position) override;
	void update(float delta) override;
	void changeMode(); // �����л�boss�Ľ�սԶ��ģʽ
	void move(); // ���ݵ�ǰģʽ����boss���ƶ�
	void attack();
};
#endif // !__BOSS_H__