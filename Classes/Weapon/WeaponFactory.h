#pragma once
#ifndef _WEAPON_FACTORY_H_
#define _WEAPON_FACTORY_H_

/*

�������ʹ������������������ֿ�����
DEMO����ʱ��ʹ�������

*/

#include"Weapon/Weapon.h"
class WeaponFactory {
public:
	static Weapon* addWeapon(std::string weaponType);
};


#endif