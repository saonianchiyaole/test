#ifndef __WAVE_MANAGER_H__
#define __WAVE_MANAGER_H__


#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"



//ֱ�۵㣬û��ĺ���
//typedef Vec2 Position;
class WaveManager :public Node {
public:
	//----------------------  VARIABLE  ----------------------
	//����
	int wave;
	//enemy����
	int currentWaveEnemyAmount;
	//��ǰ���γ���ʱ��
	float currentWaveTime;
	//��ʱ��
	float clocker;

	int waveSelectWeapon;

	bool canSelectWeapon;



	//��ʾ���ڴ�С������ÿ�ε���Director̫���������Ծʹ�������
	Vec2 visibleSize;

	//TimeManager
	TimeManager* timeManager;

	static WaveManager* instance;

	//----------------------  FUNCTION  ----------------------

	//���ɵ���
	void generateEnemy();
	//��������λ�����
	inline cocos2d::Vec2 getRandomPosition();


	//����Ƿ���Ҫ������һ�����ˣ�
	inline bool checkIfNeedToGenerateEnemy();
	//���������
	inline bool haveEnemyHaveBeenCleaned();
	//ʣ���������
	inline int residualEnemyAmount();

	static WaveManager* create();

	void callSelectWeapon();

	//��ʼ��
	bool init();

	//���ص�������ʱ���������ǣ�ȫ��ֻά��һ��WaveManager
	static WaveManager* getInstance();
	static void setInstanceToNullptr();
	void update(float dt);

};


#endif // !__WAVE_MANAGER_H__

