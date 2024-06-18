#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "config.h"
#include "Character/Character.h"

class Enemy : public Character {
public:

	/*float getSpeed() override {
		return SPEED_OF_ENEMY;
	}
	float getAD() override {
		return AD_OF_ENEMY;
	}*/

	//都写为public是为了简化getXXX()的写法，只要不随便改变量就好了（以上是之前的框架，先不删）
	////////////////////////////
	//属性区
	//由于Enemy用MoveTo保持移动，Enemy速度的实现用间隔来表示
	float interval;//移动间隔（越小移动越快）
	float bonus_exp;//击杀本怪能获得的经验值
	float attack_range; // 攻击范围

	int leftOrRight;
	////////////////////////////

	////////////////////////////
	//函数区
	Enemy();
	static Enemy* create(Vec2 position);
	static Enemy* create(Vec2 position, std::string prefixName);
	virtual bool init(Vec2 position);
	virtual bool init(Vec2 position, std::string prefixName);
	void update(float delta);//每帧调用update




	//初始化碰撞框
	void initPhysicsBody();
	//处理碰撞事件
	bool onContactBegin(PhysicsContact& contact);
	//受伤和死亡
	void hurt(int damage) override;
	void dead();
	virtual void  move(float dt);
	//死亡回调动画
	void onDeathAnimationFinished();




	//--------------TODO LIST----------------

	////////////////////////////  
};

#endif