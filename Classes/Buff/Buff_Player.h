#ifndef _BUFF_PLAYER_H
#define _BUFF_PLAYER_H

#include "Buff/Buff.h"

using namespace std;
using namespace cocos2d;

class Buff_Player :public Buff {
public:
	//�����ʼ��
	Buff_Player();
	//ֱ�Ӷ�Player���Լ�buff
	void addBuff() override;

	//������ص�buff������������ʵ�������
	const int BUFF_NUM = 5;

	float attackCapacity_delta;
	float criticalHitRate_delta;
	float defendCapacity_delta;
	float maxHP_delta;
	float curHP_delta;
};


#endif
