#ifndef _BUFF_WEAPON_H
#define _BUFF_WEAPON_H

#include "Buff/Buff.h"

using namespace std;
using namespace cocos2d;

class Buff_Weapon :public Buff {
public:
	//�����ʼ��
	Buff_Weapon();
	//ֱ�Ӷ�Player���Լ�buff
	void addBuff() override;

	//������ص�buff������������ʵ�������
	const int BUFF_NUM = 3;

	float cooldown_delta;
	float damage_delta;
	float attackTimes_delta;
};


#endif
