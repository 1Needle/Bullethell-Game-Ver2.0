#include "Stage.h"
#include <sstream>

extern sf::Vector2i mapSize;
extern sf::View view;
extern Player playerShip;
extern Enemy enemies[100];
extern int enemyCount;
extern bool win, inGame;
extern Notice notices[3];
extern int noticeCount;
extern std::stringstream ss;
extern sf::Sound stageClear;
extern sf::Music battleMusic, winMusic;

Stage::Stage(short stage)
{
	this->stage = stage;
	this->pause = true;
	this->frames = 0;
	this->spawnLimit = stage * 10;
	this->spawnCount = 0;
	this->killedEnemy = 0;
}

short Stage::getStage()
{
	return this->stage;
}
int Stage::getSpawnLimit()
{
	return this->spawnLimit;
}
int Stage::getSpawnCount()
{
	return this->spawnCount;
}
int Stage::getKilledEnemy()
{
	return this->killedEnemy;
}

void Stage::update()
{
	this->frames++;
	if (this->pause)
	{
		if (this->pauseTime == this->frames)
		{
			this->pause = false;
			this->frames = 0;
			ss << this->stage;
			notices[noticeCount] = Notice("Stage " + ss.str());
			ssClear();
		}
	}
	else
	{
		switch (stage)
		{
		case 1:
			if (this->frames == 60)
			{
				this->spawn("beam", playerShip.getCoor()+ sf::Vector2f(700, 0));
			}
			break;
		case 11:
			if (this->spawnCount <= this->spawnLimit)
			{
				if (this->frames % 120 == 0)
				{
					this->spawn("shotgun");
					this->spawn("guerilla");
				}
			}
			break;
		case 2:
			if (this->spawnCount <= this->spawnLimit)
			{
				if (this->frames % 240 == 0)
				{
					this->spawn("tank");
					this->spawn("guerilla");
				}
				else if (this->frames % 240 == 120)
				{
					this->spawn("shotgun");
					this->spawn("madman");
				}
			}
			break;
		case 3:
			if (this->spawnCount <= this->spawnLimit)
			{
				if (this->frames % 240 == 0)
				{
					this->spawn("battery");
					this->spawn("madman");
				}
				if (this->frames % 60 == 0)
				{
					this->spawn("charger");
				}
			}
			break;
		case 4:
			if (this->spawnCount <= this->spawnLimit)
			{
				if (this->frames % 240 == 0)
				{
					this->spawn("beam");
					this->spawn("mage");
				}
				else if (this->frames % 240 == 120)
				{
					this->spawn("shotgun");
					this->spawn("charger");
				}
			}
			break;
		case 5:
			if (this->spawnCount <= this->spawnLimit)
			{
				if (this->frames % 480 == 0)
				{
					this->spawn("mage");
					this->spawn("beam");
				}
				else if (this->frames % 480 == 120)
				{
					this->spawn("tank");
					this->spawn("shotgun");
					this->spawn("madman");
				}
				else if (this->frames % 480 == 240)
				{
					this->spawn("battery");
					this->spawn("guerilla");
				}
				else if (this->frames % 480 == 360)
				{
					this->spawn("charger");
					this->spawn("charger");
				}
			}
			break;
		case 6:
			if (this->spawnCount <= this->spawnLimit)
			{
				if (this->frames % 480 == 0)
				{
					this->spawn("mage");
					this->spawn("beam");
					this->spawn("beam");
				}
				else if (this->frames % 480 == 120)
				{
					this->spawn("tank");
					this->spawn("shotgun");
					this->spawn("madman");
					this->spawn("madman");
				}
				else if (this->frames % 480 == 240)
				{
					this->spawn("battery");
					this->spawn("battery");
					this->spawn("guerilla");
				}
				else if (this->frames % 480 == 360)
				{
					this->spawn("charger");
					this->spawn("charger");
					this->spawn("charger");
				}
			}
			break;
		case 7:
			if (this->spawnCount <= this->spawnLimit)
			{
				if (this->frames % 480 == 0)
				{
					this->spawn("mage");
					this->spawn("mage");
					this->spawn("beam");
					this->spawn("beam");
				}
				else if (this->frames % 480 == 120)
				{
					this->spawn("tank");
					this->spawn("shotgun");
					this->spawn("shotgun");
					this->spawn("madman");
					this->spawn("madman");
				}
				else if (this->frames % 480 == 240)
				{
					this->spawn("battery");
					this->spawn("battery");
					this->spawn("guerilla");
					this->spawn("guerilla");
				}
				else if (this->frames % 480 == 360)
				{
					this->spawn("charger");
					this->spawn("charger");
					this->spawn("charger");
					this->spawn("charger");
				}
			}
			break;
		case 8:
			if (this->spawnCount <= this->spawnLimit)
			{
				if (this->frames % 360 == 0)
				{
					this->spawn("mage");
					this->spawn("mage");
					this->spawn("beam");
					this->spawn("beam");
				}
				else if (this->frames % 360 == 90)
				{
					this->spawn("tank");
					this->spawn("shotgun");
					this->spawn("shotgun");
					this->spawn("madman");
					this->spawn("madman");
				}
				else if (this->frames % 360 == 180)
				{
					this->spawn("battery");
					this->spawn("battery");
					this->spawn("guerilla");
					this->spawn("guerilla");
				}
				else if (this->frames % 360 == 270)
				{
					this->spawn("charger");
					this->spawn("charger");
					this->spawn("charger");
					this->spawn("charger");
				}
			}
			break;
		case 9:
			if (this->spawnCount <= this->spawnLimit)
			{
				if (this->frames % 240 == 0)
				{
					this->spawn("mage");
					this->spawn("mage");
					this->spawn("beam");
					this->spawn("beam");
				}
				else if (this->frames % 240 == 60)
				{
					this->spawn("tank");
					this->spawn("shotgun");
					this->spawn("shotgun");
					this->spawn("madman");
					this->spawn("madman");
				}
				else if (this->frames % 240 == 120)
				{
					this->spawn("battery");
					this->spawn("battery");
					this->spawn("guerilla");
					this->spawn("guerilla");
				}
				else if (this->frames % 240 == 180)
				{
					this->spawn("charger");
					this->spawn("charger");
					this->spawn("charger");
					this->spawn("charger");
				}
			}
			break;
		case 10:
			if (this->frames == 120)
			{
				for (int i = 0; i < 10; i++)
				{
					this->spawn("beam", sf::Vector2f(cos(radian(i * 36)) * 750, sin(radian(i * 36)) * 750) + playerShip.getCoor());
				}
			}
			else if (this->frames == 360)
			{
				for (int i = 0; i < 5; i++)
				{
					this->spawn("mage", sf::Vector2f(cos(radian(i * 72)) * 750, sin(radian(i * 72)) * 750) + playerShip.getCoor());
				}
			}
			else if (this->frames == 940)
			{
				for (int i = 0; i < 10; i++)
				{
					this->spawn("shotgun", sf::Vector2f(cos(radian(i * 36)) * 750, sin(radian(i * 36)) * 750) + playerShip.getCoor());
					this->spawn("tank", sf::Vector2f(cos(radian(i * 36 + 18)) * 750, sin(radian(i * 36 + 18)) * 750) + playerShip.getCoor());
				}
			}
			else if (this->frames == 1540)
			{
				for (int i = 0; i < 10; i++)
				{
					this->spawn("guerilla", sf::Vector2f(cos(radian(i * 36 + 18)) * 750, sin(radian(i * 36 + 18)) * 750) + playerShip.getCoor());
				}
			}
			else if (this->frames == 1840)
			{
				for (int i = 0; i < 4; i++)
				{
					this->spawn("tank", sf::Vector2f(cos(radian(i * 90 + 45)) * 750, sin(radian(i * 90 + 45)) * 750) + playerShip.getCoor());
					this->spawn("battery", sf::Vector2f(cos(radian(i * 90)) * 750, sin(radian(i * 90)) * 750) + playerShip.getCoor());
					this->spawn("mage", sf::Vector2f(cos(radian(i * 90)) * 750, sin(radian(i * 90)) * 750) + playerShip.getCoor());
					this->spawn("beam", sf::Vector2f(cos(radian(i * 90 + 45)) * 750, sin(radian(i * 90 + 45)) * 750) + playerShip.getCoor());
					this->spawn("beam", sf::Vector2f(cos(radian(i * 90)) * 750, sin(radian(i * 90)) * 750) + playerShip.getCoor());
				}
			}
			else if (this->frames == 2140)
			{
				for (int i = 0; i < 10; i++)
				{
					this->spawn("charger", sf::Vector2f(cos(radian(i * 36 + 18)) * 750, sin(radian(i * 36 + 18)) * 750) + playerShip.getCoor());
				}
			}
			else if (this->frames == 2260)
			{
				for (int i = 0; i < 5; i++)
				{
					this->spawn("mage", sf::Vector2f(cos(radian(i * 72)) * 750, sin(radian(i * 72)) * 750) + playerShip.getCoor());
					this->spawn("battery", sf::Vector2f(cos(radian(i * 72 + 36)) * 750, sin(radian(i * 72 + 36)) * 750) + playerShip.getCoor());
				}
			}
			else if (this->frames == 2260)
			{
				for (int i = 0; i < 5; i++)
				{
					this->spawn("beam", sf::Vector2f(cos(radian(i * 72)) * 750, sin(radian(i * 72)) * 750) + playerShip.getCoor());
					this->spawn("tank", sf::Vector2f(cos(radian(i * 72)) * 750, sin(radian(i * 72)) * 750) + playerShip.getCoor());
					this->spawn("madman", sf::Vector2f(cos(radian(i * 72 + 36)) * 750, sin(radian(i * 72 + 36)) * 750) + playerShip.getCoor());
				}
			}
			else if (this->frames == 2560)
			{
				for (int i = this->spawnCount; i < this->spawnLimit; i++)
				{
					this->spawn("mage");
				}
			}
			break;
		}
	}
}
void Stage::spawn(std::string type)
{
	sf::Vector2f center = view.getCenter();
	int deviation = 30;
	float deltaX = view.getSize().x / 2 + deviation;
	float deltaY = view.getSize().y / 2 + deviation;
	float x, y;
	while (true)
	{
		x = rand() % mapSize.x;
		if (x > center.x + deltaX || x < center.x - deltaX)
		{
			if (type == "shotgun" || type == "tank")
			{
				if (abs(x - center.x) <= 800)
				{
					break;
				}
			}
			else
			{
				break;
			}
		}
	}
	while (true)
	{
		y = rand() % mapSize.y;
		if (y > center.y + deltaY || y < center.y - deltaY)
		{
			if (type == "shotgun" || type == "tank")
			{
				if (abs(y - center.y <= 500))
				{
					break;
				}
			}
			else
			{
				break;
			}
		}
	}
	enemies[enemyCount] = Enemy(type, sf::Vector2f(x, y));
	this->spawnCount++;
}
void Stage::spawn(std::string type, sf::Vector2f coor)
{
	sf::Vector2f center = view.getCenter();
	int deviation = 30;
	float deltaX = view.getSize().x / 2 + deviation;
	float deltaY = view.getSize().y / 2 + deviation;
	if ((coor.x < center.x + deltaX && coor.x > center.x - deltaX) &&
		(coor.y < center.y + deltaY && coor.y > center.y - deltaY))
	{
		return;
	}
	else
	{
		enemies[enemyCount] = Enemy(type, coor);
		this->spawnCount++;
	}
}
void Stage::killed()
{
	this->killedEnemy++;
	if (this->killedEnemy >= this->spawnLimit)
	{
		if (this->stage == 10)
		{
			win = true;
			inGame = false;
			battleMusic.stop();
			winMusic.play();
		}
		else
		{
			if (this->killedEnemy == this->spawnCount)
			{
				*this = Stage(this->stage + 1);
				stageClear.play();
				notices[noticeCount] = Notice("Stage Clear!!!");
			}
		}
	}
}