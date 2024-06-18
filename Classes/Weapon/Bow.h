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

	//ͼƬ��Դ
	static std::string pictureResourcePath;
	//ͼƬ��Դ��Ӧ�Ļ���
	//static std::string pictureCache;
	static float maxDistance;

	//---------------  FUNCTION ---------------------
	Bow();
	//�����Ĺ���
	virtual void attack();
	//���
	void shootArrow();
	void shootMultipleArrows(float dt);
	//������


	static Bow* create();
	void update(float delta);//ÿ֡����update
	bool init();
	~Bow();

	//���ǵ���ģʽ
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
	//�����ײ��
	void initPhysics();
	//������ײ�¼�
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
