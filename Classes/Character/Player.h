#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "config.h"
#include "Character/Character.h"
#include "Weapon/WeaponFactory.h"
class Player : public Character
{
public:
	/*float getSpeed() override {
		return SPEED_OF_HERO;
	}
	float getAD() override {
		return AD_OF_HERO;
	}*/

	//都写为public是为了简化getXXX()的写法，只要不随便改变量就好了（以上是之前的框架，先不删）
	////////////////////////////
	//属性区
	//最大速度，每一帧移动的像素，这是Player的实现逻辑，
	//顺便说一下Enemy的实现逻辑，由于Enemy用MoveTo保持移动，Enemy速度的实现 *用间隔来表示* 
	const float MAX_SPEED = 300.0f;
	const float velocityDelta = 30.0f;//改变速度的快慢，即加速度，可以作为 *敏捷度* 加在属性中
	float horizontalSpeed;//水平方向的速度，>0为向右，<0为向左
	float verticalSpeed;//垂直方向的速度，>0为向上，<0为向下

	//虽然实现比较臃肿，但是测试下来移动是很流畅的，所以不用管这段就行
	//移动状态枚举，
	enum MoveState {
		SUPERPOSITION, //同时按下按键
		UP, //增加状态，按下W或D
		DOWN, //减少状态，按下S或A
		NOTHING //没有按下按键
	} horizontalState, verticalState;

	int level;//角色当前等级
	float exp;//角色当前经验
	//保存按键情况
	unordered_map<EventKeyboard::KeyCode, bool>* keys;
	//角色持有的武器
	std::vector<Weapon*> weaponsPlayerOwn;
	////////////////////////////

	////////////////////////////
	//函数区
	Player();
	static Player* create(Vec2 position);
	virtual bool init(Vec2 position);
	void update(float delta);//每帧调用update
	void addKeyboardMoveEvents();//键盘监听WSAD移动事件
	void updatePosition(float dt);//更新Player位置

	void callWeaponAttack(); //调用每个武器的攻击函数
	//添加武器
	void addWeapon(std::string weaponType);
	//初始化物理碰撞
	void initPhysicsBody();
	void hurt(int damage);
	//死亡
	void dead() override;
	//监听按键情况来控制动画
	void checkKeys();


	void initBloodBar();
	void updateBloodBar(float damage);
	void initExpBar();
	void updateExpBar(float curExp, float maxExp);

	//void updateBloodBar(float damage);
	//添加动画
	virtual Animation* createWalkAnimation(const std::string& prefixName, int start, int end, float delay) override;

	//升级、经验相关函数
	float getMaxExp();//不设置等级上限，返回当前等级的最大经验值
	//处理加经验和升级事务，exp += delta_exp，调用getMaxExp()函数判断当前exp是否到达升级阈值了
	void processExp(float delta_exp);



	////////////////////////////
};

#endif