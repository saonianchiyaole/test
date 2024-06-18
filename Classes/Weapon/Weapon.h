#ifndef __WEAPON_H__
#define __WEAPON_H__

#include "config.h"
#include "Weapon/CollisionManager.h"
//#include "GameScene.h"
class Weapon : public Sprite {
public:
	//都写为public是为了简化getXXX()的写法，只要不随便改变量就好了
	////////////////////////////
	//属性区
	const int localZOrder = 3;//显示优先级，值越大越优先
	Armature* armature;//动画（如有）
	//如没有动画,即静态图，请务必添加图片资源的路径
	//std::string pictureResourcePath;
	//SpriteFrameCache* pitcureCache;
	////////////////////////////

	//冷却属性，即武器攻击的间隔
	float cooldown;
	//武器攻击的速度，与冷却区分开，冷却为每支弓箭（武器）发出的间隔，速度为弓箭（武器）的飞行速度
	float speed;
	//武器攻击次数,例如弓箭每次射出多少支
	int attackTimes;
	//武器每次造成的伤害
	int damage;
	//用来处理武器攻击间隔
	float deltaTime;
	//要服务的场景
	float attackInterval; // 单次武器攻击中每次实质攻击的间隔时间
	//用来实现单次攻击内的多个发射物数量
	int attackTimesInOneAttack;
	//碰撞管理
	static CollisionManager collisionManager;
	////////////////////////////
	//函数区
	Weapon();
	virtual ~Weapon();
	virtual bool init();
	static Weapon* create();
	//void update(float delta);
	virtual void attack();

	//查看是否已经存在某个图片缓存
	bool checkTextureCache(const std::string& textureName);
	//添加碰撞框


	////////////////////////////
};

#endif