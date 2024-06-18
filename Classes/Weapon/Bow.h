#pragma once
#ifndef _BOW_H_
#define _BOW_H_

#include"Weapon/Weapon.h"
#include"Scene/GameScene.h"
class Bow :public Weapon {
private:
	static Bow* instance;
public:
	friend class Arrow;
	friend class WeaponFactory;

	//图片资源
	static std::string pictureResourcePath;
	//图片资源对应的缓存
	//static std::string pictureCache;
	static float maxDistance;

	//---------------  FUNCTION ---------------------
	Bow();
	//弓箭的攻击
	virtual void attack();
	//射箭
	void shootArrow();
	void shootMultipleArrows(float dt);
	//老三样


	static Bow* create();
	void update(float delta);//每帧调用update
	bool init();
	~Bow();

	//还是单例模式
	static Bow* getInstance();
	static void setInstanceToNullptr() {
		instance = nullptr;
	}
};

class Arrow :public Sprite {
private:
public:

	//-----------------FUNCTION------------------------
	static Arrow* create();
	bool init();
	//添加碰撞框
	void initPhysics();
	//处理碰撞事件
	bool onContactBegin(PhysicsContact& contact);

	void update(float delta);
	void addContactListener();
	void translateAndRotate();
	//------------------- VARIABLE ----------------------
	Vec2 startPosition;
	float maxDistance;
	float dx;
	float dy;
	float speed;
	//float rotationTheta;
};

#endif // !_BOW_H_
