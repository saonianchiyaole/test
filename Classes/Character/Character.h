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
	void hurt(float);  // 被击中

	void showBloodTips(int s);  // 显示扣血数字
	void flyend(Label* label);  // 扣血数字移动并消失
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
	bool m_isAttack;  // 是否正在攻击

	bool m_ishurt;    // 被击状态

	bool alreadyAttack; //攻击已经生效了

	bool m_isAlive;  //是否还活着

	int m_life;       // 生命值

	int m_max_life;   // 最大生命值

	int lives_num; // 生命数量


	//都写为public是为了简化getXXX()的写法，只要不随便改变量就好了（以上是之前的框架，先不删）
	////////////////////////////
	//属性区
	const int localZOrder = 3;//显示优先级
	Armature* armature;//动画（如有）
	float maxHP;//最大血量值
	float curHP;//当前血量值
	float attackInterval;//攻击间隔，用于实现攻击速度
	float criticalHitRate;//暴击率
	float attackCapacity;//攻击力
	float defendCapacity;//防御力 （伤害计算公式为hurt = max(this->attackCapacity - other->defendCapacity, 0)）

	//血条
	Sprite* bloodBar;
	Sprite* bloodBarEmpty;
	////////////////////////////
	Sprite* expBar;
	Sprite* expBarEmpty;
	////////////////////////////
	//函数区
	Character();
	~Character();
	static Character* create(Vec2 position);
	virtual bool init(Vec2 position);

	//初始化血条
	virtual void initBloodBar();
	//更新血条
	virtual void updateBloodBar(float damage);
	virtual void updateBloodBar();
	//初始化碰撞框
	virtual void initPhysicsBody();
	//处理碰撞事件
	//virtual bool onContactBegin();
	virtual void hurt(int damage);
	//创建动画（用扣出来的图）
	virtual Animation* createWalkAnimation(const std::string& prefixName, int start, int end, float delay);
	virtual void createWalkAction(const std::string& prefixName, int start, int end, float delay);

	virtual void removeExpBar();


	//死亡
	virtual void dead();
	//回满血
	//virtual void fillBloodBar();
	//移除血条
	virtual void removeBloodBar();
	
	//void update(float delta);

	////////////////////////////
};

#endif