#include "Weapon/Weapon.h"
#include "Scene/GameScene.h"
class Sword :public Weapon {
private:
	static Sword* instance;
public:
	friend class Arrow;
	friend class WeaponFactory;

	//ͼƬ��Դ
	static std::string pictureResourcePath;
	//ͼƬ��Դ��Ӧ�Ļ���

	//---------------  FUNCTION ---------------------
	Sword();
	//���Ĺ���
	virtual void attack();
	//���佣��
	void shootSwordQis(float dt);
	void shootSwordQi();
	//������
	static Sword* create();
	void update(float delta);//ÿ֡����update
	bool init();
	~Sword();
	//------------------- VARIABLE ----------------------
	//�����ڳ�����������ʱ��
	static float swordQiMaxTimeInScene;
	//���ǵ���ģʽ
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
	//�����ײ��
	void initPhysics();
	//������ײ�¼�
	bool onContactBegin(PhysicsContact& contact);


	//void update(float delta);
	void addContactListener();
	void translateAndScale(int direction);

	//------------------- VARIABLE ----------------------
};