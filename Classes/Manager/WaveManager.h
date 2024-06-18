#ifndef __WAVE_MANAGER_H__
#define __WAVE_MANAGER_H__


#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"



//直观点，没别的含义
//typedef Vec2 Position;
class WaveManager :public Node {
public:
	//----------------------  VARIABLE  ----------------------
	//波次
	int wave;
	//enemy数量
	int currentWaveEnemyAmount;
	//当前波次持续时间
	float currentWaveTime;
	//计时器
	float clocker;

	int waveSelectWeapon;

	bool canSelectWeapon;



	//显示窗口大小。由于每次调用Director太耗性能所以就存起来了
	Vec2 visibleSize;

	//TimeManager
	TimeManager* timeManager;

	static WaveManager* instance;

	//----------------------  FUNCTION  ----------------------

	//生成敌人
	void generateEnemy();
	//敌人生成位置随机
	inline cocos2d::Vec2 getRandomPosition();


	//检查是否需要生成下一波敌人，
	inline bool checkIfNeedToGenerateEnemy();
	//检测敌人情况
	inline bool haveEnemyHaveBeenCleaned();
	//剩余敌人数量
	inline int residualEnemyAmount();

	static WaveManager* create();

	void callSelectWeapon();

	//初始化
	bool init();

	//返回单例，暂时不多做考虑，全局只维护一个WaveManager
	static WaveManager* getInstance();
	static void setInstanceToNullptr();
	void update(float dt);

};


#endif // !__WAVE_MANAGER_H__

