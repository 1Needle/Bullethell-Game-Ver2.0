#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include "Function.h"
#include "Animation.h"
#include "Item.h"

extern sf::RenderWindow window;
extern sf::Vector2i mapSize;
extern double radian(double);
extern int bulletCount;
extern int bombCount;
extern int beamCount;
extern int animationCount;
extern int itemCount;
extern int score;
extern sf::Sprite bulletSprite[25], bombSprite[4], beamSprite;
extern Bullet bullets[2000];
extern Bomb bombs[50];
extern Enemy enemies[100];
extern Animation animations[50];
extern Item items[50];
extern sf::Texture bulletHitTexture;
extern Player playerShip;
extern Stage stage;
extern bool lose, inGame;
extern sf::Sound beamShoot, beamStart, bombHit, playerHit, enemyHit, shieldBreak, enemyDie, playerDie;
extern sf::Music gameoverMusic, battleMusic;

Bullet::Bullet(std::string type, sf::Vector2f coor, double direction)
{
	this->type = type;
	this->direction = direction;
	this->coor = coor;
	this->exist = true;
	this->initialize();
}

bool Bullet::getHostile()
{
	return this->hostile;
}
std::string Bullet::getType()
{
	return this->type;
}
double Bullet::getSpeed()
{
	return this->speed;
}
double Bullet::getDamage()
{
	return this->damage;
}
double Bullet::getDirection()
{
	return this->direction;
}
bool Bullet::getExist()
{
	return this->exist;
}
sf::Vector2f Bullet::getCoor()
{
	return this->coor;
}
float Bullet::getX()
{
	return this->coor.x;
}
float Bullet::getY()
{
	return this->coor.y;
}
Hitbox& Bullet::getHitbox(short x)
{
	Hitbox& ref = this->hitbox[x];
	return ref;
}
int Bullet::getFrames()
{
	return this->frames;
}

void Bullet::setHostile(bool x)
{
	this->hostile = x;
}
void Bullet::setType(std::string x)
{
	this->type = x;
}
void Bullet::setSpeed(double x)
{
	this->speed = x;
}
void Bullet::setDamage(double x)
{
	this->damage = x;
}
void Bullet::setDirection(double x)
{
	while (x <= -90)
		x += 360;
	while (x > 270)
		x -= 360;
	this->direction = x;
}
void Bullet::setExist(bool x)
{
	this->exist = x;
}
void Bullet::setCoor(sf::Vector2f x)
{
	this->coor = x;
}
void Bullet::setCoor(float x, float y)
{
	this->coor.x = x;
	this->coor.y = y;
}
void Bullet::setX(float x)
{
	this->coor.x = x;
}
void Bullet::setY(float x)
{
	this->coor.y = x;
}
void Bullet::setFrames(int x)
{
	this->frames = x;
}

void Bullet::initialize()
{
	if (this->type == "level1")
	{
		this->hostile = 0;
		this->speed = 20;
		this->damage = 20;
		this->getHitbox(0).setCorner(0, -18, -6);
		this->getHitbox(0).setCorner(1, 18, -6);
		this->getHitbox(0).setCorner(2, 18, 6);
		this->getHitbox(0).setCorner(3, -18, 6);
		this->getHitbox(0).setExist(1);
	}
	else if (this->type == "level2")
	{
		this->hostile = 0;
		this->speed = 20;
		this->damage = 15;
		this->getHitbox(0).setCorner(0, -24, -9);
		this->getHitbox(0).setCorner(1, 24, -9);
		this->getHitbox(0).setCorner(2, 24, 9);
		this->getHitbox(0).setCorner(3, -24, 9);
		this->getHitbox(0).setExist(1);
	}
	else if (this->type == "level3")
	{
		this->hostile = 0;
		this->speed = 20;
		this->damage = 12;
		this->getHitbox(0).setCorner(0, -21, -18);
		this->getHitbox(0).setCorner(1, 21, -18);
		this->getHitbox(0).setCorner(2, 21, 18);
		this->getHitbox(0).setCorner(3, -21, 18);
		this->getHitbox(0).setExist(1);
	}
	else if (this->type == "homing")
	{
		this->hostile = 0;
		this->speed = 15;
		this->damage = 7;
		this->getHitbox(0).setCorner(0, -15, -12);
		this->getHitbox(0).setCorner(1, 18, -12);
		this->getHitbox(0).setCorner(2, 18, 12);
		this->getHitbox(0).setCorner(3, -15, 12);
		this->getHitbox(0).setExist(1);
	}
	else if (this->type == "redBall" || this->type == "orangeBall" ||this->type == "yellowBall" ||this->type == "greenBall" ||
		   	  this->type == "blueBall" ||this->type == "purpleBall" ||this->type == "pinkBall")
	{
		this->hostile = true;
		this->speed = 3;
		this->damage = 10;
		this->getHitbox(0).setCorner(0, -8, -8);
		this->getHitbox(0).setCorner(1, 8, -8);
		this->getHitbox(0).setCorner(2, 8, 8);
		this->getHitbox(0).setCorner(3, -8, 8);
		this->getHitbox(0).setExist(1);
	}
	else if (this->type == "redCone" || this->type == "orangeCone" || this->type == "yellowCone" || this->type == "greenCone" ||
			  this->type == "blueCone" || this->type == "purpleCone" || this->type == "pinkCone")
	{
		this->hostile = true;
		this->speed = 5;
		this->damage = 15;
		this->getHitbox(0).setCorner(0, -9, -4);
		this->getHitbox(0).setCorner(1, 5, -4);
		this->getHitbox(0).setCorner(2, 5, 4);
		this->getHitbox(0).setCorner(3, -9, 4);
		this->getHitbox(0).setExist(1);
	}
	else if (this->type == "redMagic" || this->type == "orangeMagic" || this->type == "yellowMagic" || this->type == "greenMagic" ||
			  this->type == "blueMagic" || this->type == "purpleMagic" || this->type == "pinkMagic")
	{
		this->hostile = true;
		this->speed = 5;
		this->damage = 5;
		this->getHitbox(0).setCorner(0, -11, -6);
		this->getHitbox(0).setCorner(1, 7, -6);
		this->getHitbox(0).setCorner(2, 7, 6);
		this->getHitbox(0).setCorner(3, -11, 6);
		this->getHitbox(0).setExist(1);
	}
	bulletCount++;
	if (bulletCount == 2000)
	{
		bulletCount = 0;
	}
}

void Bullet::moveset()
{
	if (this->type == "homing")
	{
		int index = findNearestEnemy(this->coor, this->direction, 60);
		if (index != -1)
		{
			this->direction = tracking(this->coor, enemies[index].getCoor(), this->direction, 1.5);
		}
	}
	else if (this->type == "blueMagic")
	{
		if (this->frames < 60)
		{
			this->setSpeed(4 * double(60 - frames) / 60);
		}
		else if (frames < 70)
		{
			this->setSpeed(0);
		}
		else if (this->frames == 70)
		{
			this->direction = faceToward(this->coor, playerShip.getCoor()) + 90;
			this->setSpeed(5);
		}
	}
}
void Bullet::update()
{
	if (!this->exist)
	{
		return;
	}
	this->frames++;
	//special type movement
	this->moveset();
	// move the bullets
	this->setX(this->getX() + float(this->speed * cos(radian(this->direction))));
	this->setY(this->getY() + float(this->speed * sin(radian(this->direction))));
	// detect if it's out of screen
	if (this->outOfScreen())
	{
		this->exist = 0;
	}
	// update hitbox
	for (auto& x : this->hitbox)
	{
		if (x.getCorner(1) == x.getCorner(2))
		{
			continue;
		}
		else
		{
			x.setCenter(this->coor);
			x.setAngle(this->direction);
			x.update();
		}
	}
	// collision detect
	if (this->hostile == false)
	{
		for (auto& x : enemies)
		{
			if (!x.getExist())
			{
				continue;
			}
			if (isCollided(x, *this))
			{
				enemyHit.play();
				x.setHealth(x.getHealth() - this->damage);
				this->exist = false;
				score += 5;
				animations[animationCount] = Animation("bulletHit", this->coor);
				if (x.getHealth() <= 0)
				{
					enemyDie.play();
					score += 100;
					stage.killed();
					x.setExist(false);
					animations[animationCount] = Animation("enemyDie", x.getCoor());
					if (rand() % 10 < 5) // 30%
					{
						items[itemCount] = Item(rand() % 4, x.getCoor());
					}
					break;
				}
			}
		}
	}
	if (this->hostile == true)
	{
		if (!playerShip.getExist())
		{
			return;
		}
		if (playerShip.getInvincible())
		{
			return;
		}
		if (isCollided(playerShip, *this))
		{
			if (playerShip.getShield())
			{
				shieldBreak.play();
				playerShip.setShield(false);
				playerShip.setInvincible(true);
				this->exist = false;
			}
			else
			{
				playerHit.play();
				playerShip.setHealth(playerShip.getHealth() - this->getDamage());
				playerShip.setInvincible(true);
				this->exist = false;
				if (playerShip.getHealth() <= 0)
				{
					playerDie.play();
					playerShip.setExist(false);
					inGame = false;
					lose = true;
					gameoverMusic.play();
					battleMusic.stop();
				}
			}
		}
	}
}
void Bullet::draw()
{
	if (!this->getExist())
	{
		return;
	}
	if (this->type == "level1")
	{
		bulletSprite[0].setPosition(this->getCoor());
		bulletSprite[0].setRotation(float(this->getDirection() + 90));
		window.draw(bulletSprite[0]);
	}
	else if (this->type == "level2")
	{
		bulletSprite[1].setPosition(this->getCoor());
		bulletSprite[1].setRotation(float(this->getDirection() + 90));
		window.draw(bulletSprite[1]);
	}
	else if (this->type == "level3")
	{
		bulletSprite[2].setPosition(this->getCoor());
		bulletSprite[2].setRotation(float(this->getDirection() + 90));
		window.draw(bulletSprite[2]);
	}
	else if (this->type == "homing")
	{
		bulletSprite[3].setScale(3, 3);
		bulletSprite[3].setPosition(this->getCoor());
		bulletSprite[3].setRotation(float(this->getDirection() + 90));
		window.draw(bulletSprite[3]);
	}
	else if (this->type == "redBall")
	{
		bulletSprite[4].setPosition(this->getCoor());
		bulletSprite[4].setRotation(float(this->getDirection() + 90));
		window.draw(bulletSprite[4]);
	}
	else if (this->type == "orangeBall")
	{
		bulletSprite[5].setPosition(this->getCoor());
		bulletSprite[5].setRotation(float(this->getDirection() + 90));
		window.draw(bulletSprite[5]);
	}
	else if (this->type == "yellowBall")
	{
		bulletSprite[6].setPosition(this->getCoor());
		bulletSprite[6].setRotation(float(this->getDirection() + 90));
		window.draw(bulletSprite[6]);
	}
	else if (this->type == "greenBall")
	{
		bulletSprite[7].setPosition(this->getCoor());
		bulletSprite[7].setRotation(float(this->getDirection() + 90));
		window.draw(bulletSprite[7]);
	}
	else if (this->type == "blueBall")
	{
		bulletSprite[8].setPosition(this->getCoor());
		bulletSprite[8].setRotation(float(this->getDirection() + 90));
		window.draw(bulletSprite[8]);
	}
	else if (this->type == "purpleBall")
	{
		bulletSprite[9].setPosition(this->getCoor());
		bulletSprite[9].setRotation(float(this->getDirection() + 90));
		window.draw(bulletSprite[9]);
	}
	else if (this->type == "pinkBall")
	{
		bulletSprite[10].setPosition(this->getCoor());
		bulletSprite[10].setRotation(float(this->getDirection() + 90));
		window.draw(bulletSprite[10]);
	}
	else if (this->type == "redCone")
	{
		bulletSprite[11].setPosition(this->getCoor());
		bulletSprite[11].setRotation(float(this->getDirection() + 90));
		window.draw(bulletSprite[11]);
	}
	else if (this->type == "orangeCone")
	{
		bulletSprite[12].setPosition(this->getCoor());
		bulletSprite[12].setRotation(float(this->getDirection() + 90));
		window.draw(bulletSprite[12]);
	}
	else if (this->type == "yellowCone")
	{
		bulletSprite[13].setPosition(this->getCoor());
		bulletSprite[13].setRotation(float(this->getDirection() + 90));
		window.draw(bulletSprite[13]);
	}
	else if (this->type == "greenCone")
	{
		bulletSprite[14].setPosition(this->getCoor());
		bulletSprite[14].setRotation(float(this->getDirection() + 90));
		window.draw(bulletSprite[14]);
	}
	else if (this->type == "blueCone")
	{
		bulletSprite[15].setPosition(this->getCoor());
		bulletSprite[15].setRotation(float(this->getDirection() + 90));
		window.draw(bulletSprite[15]);
	}
	else if (this->type == "purpleCone")
	{
		bulletSprite[16].setPosition(this->getCoor());
		bulletSprite[16].setRotation(float(this->getDirection() + 90));
		window.draw(bulletSprite[16]);
	}
	else if (this->type == "pinkCone")
	{
		bulletSprite[17].setPosition(this->getCoor());
		bulletSprite[17].setRotation(float(this->getDirection() + 90));
		window.draw(bulletSprite[17]);
	}
	else if (this->type == "redMagic")
	{
	bulletSprite[18].setPosition(this->getCoor());
	bulletSprite[18].setRotation(float(this->getDirection() + 90));
	window.draw(bulletSprite[18]);
	}
	else if (this->type == "orangeMagic")
	{
	bulletSprite[19].setPosition(this->getCoor());
	bulletSprite[19].setRotation(float(this->getDirection() + 90));
	window.draw(bulletSprite[19]);
	}
	else if (this->type == "yellowMagic")
	{
	bulletSprite[20].setPosition(this->getCoor());
	bulletSprite[20].setRotation(float(this->getDirection() + 90));
	window.draw(bulletSprite[20]);
	}
	else if (this->type == "greenMagic")
	{
	bulletSprite[21].setPosition(this->getCoor());
	bulletSprite[21].setRotation(float(this->getDirection() + 90));
	window.draw(bulletSprite[21]);
	}
	else if (this->type == "blueMagic")
	{
	bulletSprite[22].setPosition(this->getCoor());
	bulletSprite[22].setRotation(float(this->getDirection() + 90));
	window.draw(bulletSprite[22]);
	}
	else if (this->type == "purpleMagic")
	{
	bulletSprite[23].setPosition(this->getCoor());
	bulletSprite[23].setRotation(float(this->getDirection() + 90));
	window.draw(bulletSprite[23]);
	}
	else if (this->type == "pinkMagic")
	{
	bulletSprite[24].setPosition(this->getCoor());
	bulletSprite[24].setRotation(float(this->getDirection() + 90));
	window.draw(bulletSprite[24]);
	}
}
void Bullet::drawHitbox(sf::Color color)
{
	if (!this->exist)
	{
		return;
	}
	for (int i = 0; i < 4; i++)
	{
		this->getHitbox(i).draw(color);
	}
}
bool Bullet::outOfScreen() // if out of screen, then delete it
{
	if (this->getX() < -1000 || this->getX() > mapSize.x + 1000 || this->getY() < -1000 || this->getY() > mapSize.y + 1000)
	{
		return 1;
	}
	return 0;
}

Bomb::Bomb(std::string type, sf::Vector2f coor, double direction)
{
	this->setHostile(false);
	this->setType(type);
	this->setDirection(direction);
	this->setCoor(coor);
	this->setExist(true);
	this->Bomb::initialize();
}

void Bomb::initialize()
{
	if (this->getType() == "level1")
	{
		this->getHitbox(0).setCorner(0, -27, -18);
		this->getHitbox(0).setCorner(1, 27, -18);
		this->getHitbox(0).setCorner(2, 27, 18);
		this->getHitbox(0).setCorner(3, -27, 18);
		this->getHitbox(0).setExist(1);
		this->splitNum = 10;
		this->setSpeed(7);
		this->setDamage(30);
	}
	else if (this->getType() == "level2")
	{
		this->getHitbox(0).setCorner(0, -27, -31.5);
		this->getHitbox(0).setCorner(1, 27, -31.5);
		this->getHitbox(0).setCorner(2, 27, 31.5);
		this->getHitbox(0).setCorner(3, -27, 31.5);
		this->getHitbox(0).setExist(1);
		this->splitNum = 20;
		this->setSpeed(12);
		this->setDamage(50);
	}
	else if (this->getType() == "level3")
	{
		this->getHitbox(0).setCorner(0, -31.5, -36);
		this->getHitbox(0).setCorner(1, 31.5, -36);
		this->getHitbox(0).setCorner(2, 31.5, 36);
		this->getHitbox(0).setCorner(3, -31.5, 36);
		this->getHitbox(0).setExist(1);
		this->splitNum = 30;
		this->setSpeed(15);
		this->setDamage(70);
	}
	else if (this->getType() == "homing")
	{
		this->getHitbox(0).setCorner(0, -22.5, -18);
		this->getHitbox(0).setCorner(1, 27, -18);
		this->getHitbox(0).setCorner(2, 27, 18);
		this->getHitbox(0).setCorner(3, -22.5, 18);
		this->getHitbox(0).setExist(1);
		this->setSpeed(15);
		this->setDamage(10);
	}
	bombCount++;
	if (bombCount == 50)
	{
		bombCount = 0;
	}
}

void Bomb::update()
{
	if (!this->getExist())
	{
		return;
	}
	// tracking
	int index = findNearestEnemy(this->getCoor());
	if (index != -1)
	{
		this->setDirection(tracking(this->getCoor(), enemies[index].getCoor(), this->getDirection(), 3));
	}
	// move the bullets
	this->setX(this->getX() + float(this->getSpeed() * cos(radian(this->getDirection()))));
	this->setY(this->getY() + float(this->getSpeed() * sin(radian(this->getDirection()))));
	// update hitbox
	for (int i = 0; i < 4; i++)
	{
		if (!this->getHitbox(i).getExist())
		{
			continue;
		}
		else
		{
			this->getHitbox(i).setCenter(this->getCoor());
			this->getHitbox(i).setAngle(this->getDirection());
			this->getHitbox(i).update();
		}
	}
	// detect collision
	for (auto& x : enemies)
	{
		if (!x.getExist())
		{
			continue;
		}
		if (isCollided(x, *this))
		{
			enemyHit.play();
			x.setHealth(x.getHealth() - this->getDamage());
			this->setExist(false);
			this->split();
			score += 20;
			if (this->getType() == "homing")
			{
				animations[animationCount] = Animation("bulletHit", this->getCoor());
			}
			else
			{
				bombHit.play();
				animations[animationCount] = Animation("bombHit", this->getCoor());
			}
			if (x.getHealth() <= 0)
			{
				enemyDie.play();
				score += 100;
				stage.killed();
				x.setExist(false);
				animations[animationCount] = Animation("enemyDie", x.getCoor());
				if (rand() % 10 < 5) // 30%
				{
					items[itemCount] = Item(rand() % 4, x.getCoor());
				}
				return;
			}
		}
	}
	// if out of screen
	if (this->outOfScreen())
	{
		if (this->getType() != "homing")
		{
			bombHit.play();
			animations[animationCount] = Animation("bombHit", this->getCoor());
			this->split();
		}
	}
}
void Bomb::draw()
{
	if (!this->getExist())
	{
		return;
	}
	if (this->getType() == "level1")
	{
		bombSprite[0].setPosition(this->getCoor());
		bombSprite[0].setRotation(float(this->getDirection() + 90));
		window.draw(bombSprite[0]);
	}
	else if (this->getType() == "level2")
	{
		bombSprite[1].setPosition(this->getCoor());
		bombSprite[1].setRotation(float(this->getDirection() + 90));
		window.draw(bombSprite[1]);
	}
	else if (this->getType() == "level3")
	{
		bombSprite[2].setPosition(this->getCoor());
		bombSprite[2].setRotation(float(this->getDirection() + 90));
		window.draw(bombSprite[2]);
	}
	else if (this->getType() == "homing")
	{
		bombSprite[3].setPosition(this->getCoor());
		bombSprite[3].setRotation(float(this->getDirection() + 90));
		window.draw(bombSprite[3]);
	}
}
void Bomb::split()
{
	for (int i = 0; i < this->splitNum; i++)
	{
		bombs[bombCount] = Bomb("homing", this->getCoor(), 360u / this->splitNum * i);
	}
}

Beam::Beam(sf::Vector2f coor, double direction)
{
	beamStart.play();
	this->setCoor(coor);
	this->setDirection(direction);
	this->setHostile(1);
	this->setSpeed(0);
	this->setDamage(20);
	this->setExist(true);
	this->fire = false;
	beamCount++;
	if (beamCount == 20)
	{
		beamCount = 0;
	}
}
void Beam::update()
{
	if (!this->getExist())
	{
		return;
	}
	if (this->getFrames() == 30)
	{
		beamShoot.play();
		this->fire = true;
		this->getHitbox(0).setCenter(this->getCoor());
		this->getHitbox(0).setAngle(this->getDirection());
		this->getHitbox(0).setCorner(0, 0, -9);
		this->getHitbox(0).setCorner(1, 1440, -9);
		this->getHitbox(0).setCorner(2, 1440, 9);
		this->getHitbox(0).setCorner(3, 0, 9);
		this->getHitbox(0).setExist(true);
		this->getHitbox(0).update();
	}
	if (this->getFrames() == 90)
	{
		this->setExist(false);
	}
	if (this->getFrames() >= 30 && this->getFrames() < 90)
	{
		if (!playerShip.getExist())
		{
			return;
		}
		if (playerShip.getInvincible())
		{
			return;
		}
		if (isCollided(playerShip, *this))
		{
			if (playerShip.getShield())
			{
				shieldBreak.play();
				playerShip.setShield(false);
				playerShip.setInvincible(true);
			}
			else
			{
				playerHit.play();
				playerShip.setHealth(playerShip.getHealth() - this->getDamage());
				playerShip.setInvincible(true);
				if (playerShip.getHealth() <= 0)
				{
					playerDie.play();
					playerShip.setExist(false);
					inGame = false;
					lose = true;
					gameoverMusic.play();
					battleMusic.stop();
				}
			}
		}
	}
	this->setFrames(this->getFrames() + 1);
}
void Beam::draw()
{
	if (!this->getExist())
	{
		return;
	}
	if (this->fire)
	{
		beamSprite.setScale(1, 1);
	}
	else
	{
		beamSprite.setScale(1, 0.25);
	}
	beamSprite.setPosition(this->getCoor());
	beamSprite.setRotation(this->getDirection());
	window.draw(beamSprite);
}