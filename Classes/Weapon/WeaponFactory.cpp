#include "Weapon/WeaponFactory.h"
#include "Weapon/Bow.h"
#include "Weapon/Sword.h"

Weapon* WeaponFactory::addWeapon(std::string weaponType) {
	if (weaponType == "Bow" || weaponType == "bow") {
		return Bow::getInstance();
	}
	else if (weaponType == "Sword" || weaponType == "sword") {
		return Sword::getInstance();
	}
	else return nullptr;

}
