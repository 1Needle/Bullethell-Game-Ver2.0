#include <SFML/Graphics.hpp>
#include "Animation.h"
#include <iostream>

extern sf::RenderWindow window;
extern int animationCount;
extern sf::Sprite bulletHitSprite, bombHitSprite, enemyDieSprite, shieldBreakSprite;

Animation::Animation(std::string type, sf::Vector2f coor)
{
	this->type = type;
	this->coor = coor;
	this->exist = true;
	this->initialize();
}
Animation::Animation(std::string type, sf::Vector2f coor, int delay)
{
	this->delay = delay;
	this->type = type;
	this->coor = coor;
	this->exist = true;
	this->initialize();
}

void Animation::setCoor(float x, float y)
{
	this->coor.x = x;
	this->coor.y = y;
}
void Animation::setCoor(sf::Vector2f x)
{
	this->coor = x;
}
void Animation::setExist(bool x)
{
	this->exist = x;
}

void Animation::initialize()
{
	if (this->type == "bulletHit")
	{
		this->maxFrame = 5;
		this->maxIndex = 3;
	}
	else if (this->type == "bombHit")
	{
		this->maxFrame = 10;
		this->maxIndex = 3;
	}
	else if (this->type == "enemyDie")
	{
		this->maxFrame = 12;
		this->maxIndex = 3;
	}
	else if (this->type == "shieldBreak")
	{
		this->maxFrame = 30;
	}
	else
	{
		std::cout << "undefined animation type : " << this->type << std::endl;
	}
	animationCount++;
	if (animationCount == 50)
	{
		animationCount = 0;
	}
}
void Animation::update()
{
	if (!this->exist)
	{
		return;
	}
	if (delay != 0)
	{
		delay--;
		return;
	}
	else
	{
		this->frame++;
	}
	if (this->type == "bulletHit" || this->type == "bombHit" || this->type == "enemyDie")
	{
		if (this->frame == this->maxFrame)
		{
			this->index++;
			this->frame = 0;
			if (this->index == this->maxIndex)
			{
				if (repeat)
				{
					this->index = 0;
				}
				else
				{
					this->exist = 0;
				}
			}
		}
	}
	else if (this->type == "shieldBreak")
	{
		if (this->frame == this->maxFrame)
		{
			this->exist = 0;
		}
	}
}
void Animation::draw()
{
	if (!this->exist || this->delay != 0)
	{
		return;
	}
	if (this->type == "bulletHit")
	{
		bulletHitSprite.setTextureRect(sf::IntRect(this->index * 16, 0, 16, 16));
		bulletHitSprite.setPosition(this->coor);
		window.draw(bulletHitSprite);
	}
	else if (this->type == "bombHit")
	{
		bombHitSprite.setTextureRect(sf::IntRect(this->index * 16, 0, 16, 16));
		bombHitSprite.setPosition(this->coor);
		window.draw(bombHitSprite);
	}
	else if (this->type == "enemyDie")
	{
		enemyDieSprite.setTextureRect(sf::IntRect(this->index * 32, 0, 32, 32));
		enemyDieSprite.setPosition(this->coor);
		window.draw(enemyDieSprite);
	}
	else if (this->type == "shieldBreak")
	{
		shieldBreakSprite.setColor(sf::Color(255, 255, 255, int(double(this->maxFrame - this->frame) / this->maxFrame * 255)));
		shieldBreakSprite.setScale(float(this->frame) / this->maxFrame * 4, float(this->frame) / this->maxFrame * 4);
		shieldBreakSprite.setPosition(this->coor);
		window.draw(shieldBreakSprite);
	}
}