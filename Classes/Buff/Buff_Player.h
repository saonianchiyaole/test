#ifndef _BUFF_PLAYER_H
#define _BUFF_PLAYER_H

#include "Buff/Buff.h"

using namespace std;
using namespace cocos2d;

class Buff_Player :public Buff {
public:
	//随机初始化
	Buff_Player();
	//直接对Player属性加buff
	void addBuff() override;

	//人物相关的buff种类数（用于实现随机）
	const int BUFF_NUM = 5;

	float attackCapacity_delta;
	float criticalHitRate_delta;
	float defendCapacity_delta;
	float maxHP_delta;
	float curHP_delta;
};


#endif
