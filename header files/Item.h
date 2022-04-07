#pragma once
#include <SFML/Graphics.hpp>
#include "Hitbox.h"

class Item
{
private:
	sf::Vector2f coor;
	short type = 0;
	double direction = 0;
	double speed = 0, horizontalSpeed = 0, verticalSpeed = 0;
	double brake = 0.9;
	double minSpeed = 1;
	bool xHit = 0, yHit = 0;
	bool exist = 0;
	sf::Vector2f size;
	Hitbox hitbox;
	void initialize();
public:
	Item() = default;
	Item(short, sf::Vector2f);

	sf::Vector2f getCoor();
	short getType();
	double getDirection();
	double getSpeed();
	double getBrake();
	double getMinSpeed();
	bool getExist();
	Hitbox& getHitbox();

	void setCoor(sf::Vector2f);
	void setCoor(float, float);
	void setDirection(double);
	void setSpeed(double);
	void setExist(bool);

	void update();
	void move(double);
	void pickDetect();
	void bounceDetect();
	void draw();
	void drawHitbox(sf::Color);
};