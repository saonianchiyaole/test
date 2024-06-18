#include"Character/MeleeEnemy.h"
#include"Character/RangedEnemy.h"
#include"Character/Boss.h"

class EnemyFactory {
public:
	static EnemyFactory* instance;
	static Enemy* addEnemy(std::string enemyType, Vec2 position);
	static Enemy* addEnemy(Vec2 position, int enemyType);
};