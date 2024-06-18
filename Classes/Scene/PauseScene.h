#ifndef __PAUSE_SCENE_H__
#define __PAUSE_SCENE_H__

#include "cocos2d.h"

class PauseScene : public cocos2d::Scene {
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	void toggleVolume();
	void returnToGame();
	CREATE_FUNC(PauseScene);
};

#endif // __PAUSE_SCENE_H__
