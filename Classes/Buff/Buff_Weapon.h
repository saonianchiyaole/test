#ifndef _BUFF_WEAPON_H
#define _BUFF_WEAPON_H

#include "Buff/Buff.h"

using namespace std;
using namespace cocos2d;

class Buff_Weapon :public Buff {
public:
	//随机初始化
	Buff_Weapon();
	//直接对Player属性加buff
	void addBuff() override;

	//武器相关的buff种类数（用于实现随机）
	const int BUFF_NUM = 3;

	float cooldown_delta;
	float damage_delta;
	float attackTimes_delta;
};


#endif
