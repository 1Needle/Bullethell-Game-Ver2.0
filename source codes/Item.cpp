#include <SFML/Graphics.hpp>
#include "Item.h"
#include "Function.h"
#include "Character.h"
#include <iostream>

extern sf::RenderWindow window;
extern sf::Sprite itemSprite[4];
extern sf::Vector2i mapSize;
extern int itemCount;
extern Player playerShip;
extern int score;
extern sf::Sound getHealth, getPower, getBomb, getShield;

void Item::initialize()
{
	switch (this->type)
	{
	default:
		break;
	case 0:
		this->size = sf::Vector2f(32, 24);
		break;
	case 1:
		this->size = sf::Vector2f(24, 29);
		break;
	case 2:
		this->size = sf::Vector2f(24, 24);
		break;
	case 3:
		this->size = sf::Vector2f(32, 32);
		break;
	}
	hitbox.setCorner(0, sf::Vector2f(this->size.x / -2, this->size.y / -2));
	hitbox.setCorner(1, sf::Vector2f(this->size.x / 2, this->size.y / -2));
	hitbox.setCorner(2, sf::Vector2f(this->size.x / 2, this->size.y / 2));
	hitbox.setCorner(3, sf::Vector2f(this->size.x / -2, this->size.y / 2));
	hitbox.setExist(1);
	this->move(rand() % 360);
	itemCount++;
	if (itemCount == 50)
	{
		itemCount = 0;
	}
}

Item::Item(short type, sf::Vector2f coor)
{
	this->type = type;
	this->coor = coor;
	this->exist = 1;
	hitbox.setCenter(coor);
	this->initialize();
}

sf::Vector2f Item::getCoor()
{
	return this->coor;
}
short Item::getType()
{
	return this->type;
}
double Item::getDirection()
{
	return this->direction;
}
double Item::getSpeed()
{
	return this->speed;
}
double Item::getBrake()
{
	return this->brake;
}
double Item::getMinSpeed()
{
	return this->minSpeed;
}
bool Item::getExist()
{
	return this->exist;
}
Hitbox& Item::getHitbox()
{
	Hitbox& ref = this->hitbox;
	return ref;
}

void Item::setCoor(sf::Vector2f x)
{
	this->coor = x;
}
void Item::setCoor(float x, float y)
{
	this->coor.x = x;
	this->coor.y = y;
}
void Item::setDirection(double x)
{
	this->direction = x;
}
void Item::setSpeed(double x)
{
	this->speed = x;
}
void Item::setExist(bool x)
{
	this->exist = x;
}

void Item::update()
{
	if (!this->getExist())
	{
		return;
	}
	// update position
	if (this->speed < this->minSpeed)
	{
		this->speed = 0;
	}
	else
	{
		this->speed *= this->brake;
	}
	if (this->speed != 0)
	{
		this->horizontalSpeed = cos(radian(this->direction)) * this->speed;
		this->verticalSpeed = sin(radian(this->direction)) * this->speed;
		this->coor.x += float(this->horizontalSpeed);
		this->coor.y += float(this->verticalSpeed);
		this->bounceDetect();
	}
	// update hitbox
	this->hitbox.setCenter(this->coor);
	this->hitbox.update();
	// pick detection
	this->pickDetect();
}
void Item::move(double direction)
{
	this->direction = direction;
	this->speed = 20;
}
void Item::pickDetect()
{
	if (this->hitbox.isCollided(playerShip.getHitbox(1)) || this->hitbox.isCollided(playerShip.getHitbox(0)))
	{
		switch (this->type)
		{
		case 0:
			if (playerShip.getHealth() != playerShip.getMaxHealth())
			{
				getHealth.play();
				this->exist = 0;
				if (playerShip.getHealth() + 10 < playerShip.getMaxHealth())
				{
					playerShip.setHealth(playerShip.getHealth() + 10);
				}
				else
				{
					playerShip.setHealth(playerShip.getMaxHealth());
				}
			}
			break;
		case 1:
			getPower.play();
			if (playerShip.getPower() != playerShip.getMaxPower())
			{
				this->exist = 0;
				playerShip.setPower(playerShip.getPower() + 1);
			}
			else
			{
				this->exist = 0;
				score += 500;
			}
			break;
		case 2:
			getShield.play();
			playerShip.setShield(true);
			this->exist = false;
			break;
		case 3:
			if (playerShip.getBomb() != playerShip.getMaxBomb())
			{
				getBomb.play();
				this->exist = 0;
				playerShip.setBomb(playerShip.getBomb() + 1);
			}
		}
	}
}
void Item::bounceDetect()
{
	if (this->xHit == 0)
	{
		if (this->coor.x - this->size.x / 2 <= 0 || this->coor.x + this->size.x / 2 >= mapSize.x)
		{
			this->direction += (270 - this->direction) * 2;
			xHit = 1;
		}
	}
	if (this->yHit == 0)
	{
		if (this->coor.y - this->size.y / 2 <= 0 || this->coor.y + this->size.y / 2 >= mapSize.y)
		{
			this->direction += (360 - this->direction) * 2;
			yHit = 1;
		}
	}
}
void Item::draw()
{
	if (!this->exist)
	{
		return;
	}
	itemSprite[this->type].setPosition(this->coor);
	window.draw(itemSprite[this->type]);
}
void Item::drawHitbox(sf::Color color)
{
	if (!this->exist)
	{
		return;
	}
	this->hitbox.draw(color);
}