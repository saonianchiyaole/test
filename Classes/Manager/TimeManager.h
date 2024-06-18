
#ifndef __TIME_MANAGER_H__
#define __TIME_MANAGER_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"

class TimeManager :cocos2d::Node {
public:
	//----------------------  VARIABLE  ----------------------
	float timer;
	float waveTime;
	float inGameTime;
	bool ifInGameScene;
	bool ifWaveStart;
	//----------------------  FUNCTION  ----------------------
	static TimeManager* getInstance();
	void update(float dt);

	float getTimer();
	float getWaveTime();
	float getInGameTime();

	inline void setTimer(float t);
	void setWaveTime(float t);
	void setInGameTime(float t);

	bool init();

	static TimeManager* create();

private:
	//µ¥Àý
	static TimeManager* instance;	



	//ÓÑÔªÀà
	friend class WaveManager;
};


#endif // !__TIME_MANAGER_H__
