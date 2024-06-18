#include "Weapon/Weapon.h"
#include "Scene/GameScene.h"
class Sword :public Weapon {
private:
	static Sword* instance;
public:
	friend class Arrow;
	friend class WeaponFactory;

	//图片资源
	static std::string pictureResourcePath;
	//图片资源对应的缓存

	//---------------  FUNCTION ---------------------
	Sword();
	//剑的攻击
	virtual void attack();
	//发射剑气
	void shootSwordQis(float dt);
	void shootSwordQi();
	//老三样
	static Sword* create();
	void update(float delta);//每帧调用update
	bool init();
	~Sword();
	//------------------- VARIABLE ----------------------
	//剑气在场景中最大存在时间
	static float swordQiMaxTimeInScene;
	//还是单例模式
	static Sword* getInstance();
	static void setInstanceToNullptr() {
		instance = nullptr;
	}
};

class SwordQi :public Sprite {
public:

	//-----------------FUNCTION------------------------
	static SwordQi* create();
	bool init();
	//添加碰撞框
	void initPhysics();
	//处理碰撞事件
	bool onContactBegin(PhysicsContact& contact);


	//void update(float delta);
	void addContactListener();
	void translateAndScale(int direction);

	//------------------- VARIABLE ----------------------
};