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

	//��дΪpublic��Ϊ�˼�getXXX()��д����ֻҪ�����ı����ͺ��ˣ�������֮ǰ�Ŀ�ܣ��Ȳ�ɾ��
	////////////////////////////
	//������
	//����ٶȣ�ÿһ֡�ƶ������أ�����Player��ʵ���߼���
	//˳��˵һ��Enemy��ʵ���߼�������Enemy��MoveTo�����ƶ���Enemy�ٶȵ�ʵ�� *�ü������ʾ* 
	const float MAX_SPEED = 300.0f;
	const float velocityDelta = 30.0f;//�ı��ٶȵĿ����������ٶȣ�������Ϊ *���ݶ�* ����������
	float horizontalSpeed;//ˮƽ������ٶȣ�>0Ϊ���ң�<0Ϊ����
	float verticalSpeed;//��ֱ������ٶȣ�>0Ϊ���ϣ�<0Ϊ����

	//��Ȼʵ�ֱȽ�ӷ�ף����ǲ��������ƶ��Ǻ������ģ����Բ��ù���ξ���
	//�ƶ�״̬ö�٣�
	enum MoveState {
		SUPERPOSITION, //ͬʱ���°���
		UP, //����״̬������W��D
		DOWN, //����״̬������S��A
		NOTHING //û�а��°���
	} horizontalState, verticalState;

	int level;//��ɫ��ǰ�ȼ�
	float exp;//��ɫ��ǰ����
	//���水�����
	unordered_map<EventKeyboard::KeyCode, bool>* keys;
	//��ɫ���е�����
	std::vector<Weapon*> weaponsPlayerOwn;
	////////////////////////////

	////////////////////////////
	//������
	Player();
	static Player* create(Vec2 position);
	virtual bool init(Vec2 position);
	void update(float delta);//ÿ֡����update
	void addKeyboardMoveEvents();//���̼���WSAD�ƶ��¼�
	void updatePosition(float dt);//����Playerλ��

	void callWeaponAttack(); //����ÿ�������Ĺ�������
	//�������
	void addWeapon(std::string weaponType);
	//��ʼ��������ײ
	void initPhysicsBody();
	void hurt(int damage);
	//����
	void dead() override;
	//����������������ƶ���
	void checkKeys();


	void initBloodBar();
	void updateBloodBar(float damage);
	void initExpBar();
	void updateExpBar(float curExp, float maxExp);

	//void updateBloodBar(float damage);
	//��Ӷ���
	virtual Animation* createWalkAnimation(const std::string& prefixName, int start, int end, float delay) override;

	//������������غ���
	float getMaxExp();//�����õȼ����ޣ����ص�ǰ�ȼ��������ֵ
	//����Ӿ������������exp += delta_exp������getMaxExp()�����жϵ�ǰexp�Ƿ񵽴�������ֵ��
	void processExp(float delta_exp);



	////////////////////////////
};

#endif