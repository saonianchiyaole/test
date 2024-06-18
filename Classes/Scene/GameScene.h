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




	//��дΪpublic��Ϊ�˷�ֹĳ�����ɷ��ʣ��������ɶȣ�ֻҪ�����ı����ͺ���
	////////////////////////////
	
	//---------------------- BUFF ------------------------------
	//�����������Ƿ������Ӱ�ť��
	bool isAddButtonOk;
	//��ť������(Ϊ��ֹ����������ťһ����չʾ����
	int buttonBufferNum;
	//-----------------------------------------------------------
	//�����жϱ���

	bool isGameOver;
	bool havePlayerWin;
	bool isGameStop;
	bool havePlayerSelectWeapon;


	bool isDisplayingOption;

	//������
	Player* player;
	//std::list<Enemy*> enemyByList;
	std::unordered_set<Enemy*> enemyByHashSet;
	//���λ��
	Vec2 mousePosition;
	
	//������� true Ϊ���£�falseΪû����
	unordered_map<EventKeyboard::KeyCode, bool> keys;

	Buff* buff;//Ψһ��ѡ�е�buff
	Buff_Player* buff_player;
	Buff_Weapon* buff_weapon;

	//һ�ֵ�ʱ��
	float maxTimePerGame;
	
	//---------------- Component ----------------
	Label* waveNumLabel;
	Label* timerLabel;
	Sprite* expBar;
	Sprite* expBarEmpty;

	//------------  Manager -----------------
	WaveManager* waveManager;
	//��ʱ���ĵ���
	TimeManager* timeManager;
	//��ʱ����UIManager
	//UIManager* uiManager;
	////////////////////////////

	////////////////////////////
	//������

	//���ص���
	static GameScene* getInstance();
	//�������λ��
	void onMouseMove(Event* event);
	//������λ���������� ��������Ǹ�
	void addMouseMoveListener();
	//��ʼ������״��
	void initKeys();



	//----------------------------- ----------------------------------
	//��ʼ������
	void initVariable();

	//��ײ���
	bool onContactBegin(PhysicsContact& contact);
	//�����ײ��������
	void addContactListener();
	//��Ӽ��̼�����
	void addKeyBoardListener();
	//��Ӳ��ι�����
	void addWaveManager();
	//��ӹ���
	bool addEnemy(Enemy* newEnemy);
	//�Ƴ�����
	bool removeEnemy(Enemy* enemy);
	//��HashSet���Ƴ�enemy
	bool removeEnemyFromHashSet(Enemy* enemy);
	//---------------------- BUFF ------------------------------
	//��ʾѡ��
	void displayOptions();
	void buffCallback();
	
	//����ѡ��
	void updateOptions();

	//��ѡ���������Ʈ�Ķ���
	void addActionToOption(int tag);
	void flyend();

	//-----------------------  COMPONENT ---------------------------
	//ʱ����ʾ
	inline void drawTimer(int minute, int second);
	//����
	void drawWaveNum(int num);
	//����ʱ��
	inline void updateTimer();
	//���²���
	void updateWaveNum(int num);
	//���������
	void updateExpBar();
	void drawExpBar();
	//��ӹرհ�ť
	void addCloseItem();
	//��ӱ���
	void addBackground();

	//��ʼ�����ָ��
	inline void initComponentPointer();
	//��ʼ�����
	void initComponent();

	// ------------------------- SHADER  --------------------------------
	void initRedShader();

	//---------------------------GAME PLAYER ------------------------------------
	void gameOver(bool winOrNot);
	void drawSelectWeaponButton();
	void selectWeapon(std::string weaponType);
	
	//---------------------------- SYSTEM -----------------------------------
	//��Ҫ����
	static void setInstanceToNullptr();
	

	void update(float dt);


	//��ͣ��Ϸ
	void pauseGame();
	////////////////////////////


	//��Ԫ������
	friend class WaveManager;


};

#endif // __GAME_SCENE_H__
