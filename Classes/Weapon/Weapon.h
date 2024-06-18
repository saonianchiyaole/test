#ifndef __WEAPON_H__
#define __WEAPON_H__

#include "config.h"
#include "Weapon/CollisionManager.h"
//#include "GameScene.h"
class Weapon : public Sprite {
public:
	//��дΪpublic��Ϊ�˼�getXXX()��д����ֻҪ�����ı����ͺ���
	////////////////////////////
	//������
	const int localZOrder = 3;//��ʾ���ȼ���ֵԽ��Խ����
	Armature* armature;//���������У�
	//��û�ж���,����̬ͼ����������ͼƬ��Դ��·��
	//std::string pictureResourcePath;
	//SpriteFrameCache* pitcureCache;
	////////////////////////////

	//��ȴ���ԣ������������ļ��
	float cooldown;
	//�����������ٶȣ�����ȴ���ֿ�����ȴΪÿ֧�����������������ļ�����ٶ�Ϊ�������������ķ����ٶ�
	float speed;
	//������������,���繭��ÿ���������֧
	int attackTimes;
	//����ÿ����ɵ��˺�
	int damage;
	//�������������������
	float deltaTime;
	//Ҫ����ĳ���
	float attackInterval; // ��������������ÿ��ʵ�ʹ����ļ��ʱ��
	//����ʵ�ֵ��ι����ڵĶ������������
	int attackTimesInOneAttack;
	//��ײ����
	static CollisionManager collisionManager;
	////////////////////////////
	//������
	Weapon();
	virtual ~Weapon();
	virtual bool init();
	static Weapon* create();
	//void update(float delta);
	virtual void attack();

	//�鿴�Ƿ��Ѿ�����ĳ��ͼƬ����
	bool checkTextureCache(const std::string& textureName);
	//�����ײ��


	////////////////////////////
};

#endif