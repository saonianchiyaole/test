/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "config.h"
#include "cocos2d.h"
#include "Character/Player.h"
#include "Character/Enemy.h"
#include "Buff/Buff_Player.h"
#include "Buff/Buff_Weapon.h"
#include "Manager/WaveManager.h"
#include "Manager/EnemyFactory.h"


class GameScene : public cocos2d::Scene
{
private:
	static GameScene* instance;
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// a selector callback
	void gameoverCallback(cocos2d::Ref* pSender);

	// implement the "static create()" method manually
	static GameScene* create() {
		GameScene* pRet = GameScene::getInstance();
		if (pRet && pRet->init()) {
			pRet->autorelease();
			return pRet;
		}
		else {
			delete pRet;
			pRet = nullptr;
			return nullptr;
		}
	};

	//--------------------- VARIABLE -----------------------




	//都写为public是为了防止某处不可访问，增大自由度，只要不随便改变量就好了
	////////////////////////////
	
	//---------------------- BUFF ------------------------------
	//布尔变量：是否可以添加按钮了
	bool isAddButtonOk;
	//按钮缓冲区(为防止升级跳出按钮一次性展示出来
	int buttonBufferNum;
	//-----------------------------------------------------------
	//条件判断变量

	bool isGameOver;
	bool havePlayerWin;
	bool isGameStop;
	bool havePlayerSelectWeapon;


	bool isDisplayingOption;

	//属性区
	Player* player;
	//std::list<Enemy*> enemyByList;
	std::unordered_set<Enemy*> enemyByHashSet;
	//鼠标位置
	Vec2 mousePosition;
	
	//按键情况 true 为按下，false为没按下
	unordered_map<EventKeyboard::KeyCode, bool> keys;

	Buff* buff;//唯一被选中的buff
	Buff_Player* buff_player;
	Buff_Weapon* buff_weapon;

	//一局的时间
	float maxTimePerGame;
	
	//---------------- Component ----------------
	Label* waveNumLabel;
	Label* timerLabel;
	Sprite* expBar;
	Sprite* expBarEmpty;

	//------------  Manager -----------------
	WaveManager* waveManager;
	//计时器的单例
	TimeManager* timeManager;
	//暂时不用UIManager
	//UIManager* uiManager;
	////////////////////////////

	////////////////////////////
	//函数区

	//返回单例
	static GameScene* getInstance();
	//返回鼠标位置
	void onMouseMove(Event* event);
	//添加鼠标位置侦听器， 针对上面那个
	void addMouseMoveListener();
	//初始化按键状况
	void initKeys();



	//----------------------------- ----------------------------------
	//初始化变量
	void initVariable();

	//碰撞检测
	bool onContactBegin(PhysicsContact& contact);
	//添加碰撞检测监听器
	void addContactListener();
	//添加键盘监听器
	void addKeyBoardListener();
	//添加波次管理器
	void addWaveManager();
	//添加怪物
	bool addEnemy(Enemy* newEnemy);
	//移除怪物
	bool removeEnemy(Enemy* enemy);
	//从HashSet中移除enemy
	bool removeEnemyFromHashSet(Enemy* enemy);
	//---------------------- BUFF ------------------------------
	//显示选项
	void displayOptions();
	void buffCallback();
	
	//更新选项
	void updateOptions();

	//给选项加上向上飘的动作
	void addActionToOption(int tag);
	void flyend();

	//-----------------------  COMPONENT ---------------------------
	//时间显示
	inline void drawTimer(int minute, int second);
	//波次
	void drawWaveNum(int num);
	//更新时间
	inline void updateTimer();
	//更新波次
	void updateWaveNum(int num);
	//经验条相关
	void updateExpBar();
	void drawExpBar();
	//添加关闭按钮
	void addCloseItem();
	//添加背景
	void addBackground();

	//初始化组件指针
	inline void initComponentPointer();
	//初始化组件
	void initComponent();

	// ------------------------- SHADER  --------------------------------
	void initRedShader();

	//---------------------------GAME PLAYER ------------------------------------
	void gameOver(bool winOrNot);
	void drawSelectWeaponButton();
	void selectWeapon(std::string weaponType);
	
	//---------------------------- SYSTEM -----------------------------------
	//必要函数
	static void setInstanceToNullptr();
	

	void update(float dt);


	//暂停游戏
	void pauseGame();
	////////////////////////////


	//友元类声明
	friend class WaveManager;


};

#endif // __GAME_SCENE_H__
