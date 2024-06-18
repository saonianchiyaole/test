#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include "config.h"

enum CategoryBitMask {
	PLAYER = 0x1,
	ENEMY = 0x2,
	ATTACK = 0x3,
	UNKNOWN = 0x4
};

const int maxLife = 50;

class Character : public Sprite {
public:
	void setMagic() { isMagicAttack = true; }
	void cancelMagic() { isMagicAttack = false; }

	//void play(SignalState state);
	void hurt(float);  // ������

	void showBloodTips(int s);  // ��ʾ��Ѫ����
	void flyend(Label* label);  // ��Ѫ�����ƶ�����ʧ
	void onFrameEvent(cocostudio::Bone* bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);

	// set and get
	Armature* getArmature() { return m_armature; }

	bool isAttack() { return m_isAttack; }
	void setAttack(bool attack) { m_isAttack = attack; }

	int getLife() { return m_life; }
	void setLife(int life) { m_life = life; }

	void attackDone() { alreadyAttack = true; }
	bool hasAlreadyAttack() { return alreadyAttack; }

	//bool isDeath() { return m_characterState==CharacterState::DEATH; }

	void fullLife() { m_life = m_max_life; }
	int getMaxLife() { return m_max_life; }
	void setLivesNum(int _num) { lives_num = _num; }
	int getLivesNum() { return lives_num; }
	void revive() { lives_num--; m_life = m_max_life; }
	//CharacterState getState() { return m_characterState; }

	virtual float getSpeed() {
		return 0.1f;
	}
	virtual float getAD() {
		return 0.1f;
	}
	bool attackEnd;

	Armature* m_armature;
	//SignalState m_signalState;
	//CharacterState m_characterState;
	RepeatForever* moveAction;


	bool isMagicAttack;

	bool m_isrunning;
	bool m_isdead;
	bool m_isAttack;  // �Ƿ����ڹ���

	bool m_ishurt;    // ����״̬

	bool alreadyAttack; //�����Ѿ���Ч��

	bool m_isAlive;  //�Ƿ񻹻���

	int m_life;       // ����ֵ

	int m_max_life;   // �������ֵ

	int lives_num; // ��������


	//��дΪpublic��Ϊ�˼�getXXX()��д����ֻҪ�����ı����ͺ��ˣ�������֮ǰ�Ŀ�ܣ��Ȳ�ɾ��
	////////////////////////////
	//������
	const int localZOrder = 3;//��ʾ���ȼ�
	Armature* armature;//���������У�
	float maxHP;//���Ѫ��ֵ
	float curHP;//��ǰѪ��ֵ
	float attackInterval;//�������������ʵ�ֹ����ٶ�
	float criticalHitRate;//������
	float attackCapacity;//������
	float defendCapacity;//������ ���˺����㹫ʽΪhurt = max(this->attackCapacity - other->defendCapacity, 0)��

	//Ѫ��
	Sprite* bloodBar;
	Sprite* bloodBarEmpty;
	////////////////////////////
	Sprite* expBar;
	Sprite* expBarEmpty;
	////////////////////////////
	//������
	Character();
	~Character();
	static Character* create(Vec2 position);
	virtual bool init(Vec2 position);

	//��ʼ��Ѫ��
	virtual void initBloodBar();
	//����Ѫ��
	virtual void updateBloodBar(float damage);
	virtual void updateBloodBar();
	//��ʼ����ײ��
	virtual void initPhysicsBody();
	//������ײ�¼�
	//virtual bool onContactBegin();
	virtual void hurt(int damage);
	//�����������ÿ۳�����ͼ��
	virtual Animation* createWalkAnimation(const std::string& prefixName, int start, int end, float delay);
	virtual void createWalkAction(const std::string& prefixName, int start, int end, float delay);

	virtual void removeExpBar();


	//����
	virtual void dead();
	//����Ѫ
	//virtual void fillBloodBar();
	//�Ƴ�Ѫ��
	virtual void removeBloodBar();
	
	//void update(float delta);

	////////////////////////////
};

#endif