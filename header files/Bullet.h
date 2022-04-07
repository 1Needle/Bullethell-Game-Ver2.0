#pragma once
#include <SFML/Graphics.hpp>
#include "Hitbox.h"

class Bullet
{
private:
	bool hostile = 0;
	std::string type;
	double speed = 0;
	double damage = 0;
	double direction = 0;
	bool exist = 0;
	int frames = 0;
	sf::Vector2f coor = sf::Vector2f(0, 0);
	Hitbox hitbox[4];

	void initialize();
public:
	Bullet() = default;
	Bullet(std::string type, sf::Vector2f coor, double direction);

	bool getHostile();
	std::string getType();
	double getSpeed();
	double getDamage();
	double getDirection();
	bool getExist();
	sf::Vector2f getCoor();
	float getX();
	float getY();
	Hitbox& getHitbox(short);
	int getFrames();

	void setHostile(bool);
	void setType(std::string);
	void setSpeed(double);
	void setDamage(double);
	void setDirection(double);
	void setExist(bool);
	void setCoor(sf::Vector2f);
	void setCoor(float, float);
	void setX(float);
	void setY(float);
	void setFrames(int);

	void moveset();
	void update();
	void draw();
	void drawHitbox(sf::Color);
	bool outOfScreen();
};

class Bomb : public Bullet
{
private:
	short splitNum = 0;
	void initialize();
public:
	Bomb() = default;
	Bomb(std::string type, sf::Vector2f coor, double direction);
	void update();
	void draw();
	void split();
};

class Beam : public Bullet
{
private:
	bool fire = 0;
public:
	Beam() = default;
	Beam(sf::Vector2f coor, double direction);
	void update();
	void draw();
};