#pragma once
#ifndef _WEAPON_FACTORY_H_
#define _WEAPON_FACTORY_H_

/*

将武器和创建武器这两个东西分开来。
DEMO中暂时不使用这个类

*/

#include"Weapon/Weapon.h"
class WeaponFactory {
public:
	static Weapon* addWeapon(std::string weaponType);
};


#endif