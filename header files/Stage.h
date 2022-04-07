#pragma once
#include "Function.h"
class Stage
{
private:
	short stage = 1;
	int spawnLimit = 0;
	int spawnCount = 0;
	int frames = 0;
	int killedEnemy = 0;
	int pauseTime = 180;
	bool pause = 0;
public:
	Stage() = default;
	Stage(short stage);

	short getStage();
	int getSpawnLimit();
	int getSpawnCount();
	int getKilledEnemy();

	void update();
	void spawn(std::string type);
	void spawn(std::string type, sf::Vector2f coor);
	void killed();
};