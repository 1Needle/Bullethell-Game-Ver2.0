#pragma once
#include <SFML/Graphics.hpp>

class Hitbox
{
private:
	sf::Vector2f center = sf::Vector2f(0, 0);
	sf::Vector2f corner[4] = { sf::Vector2f(0, 0) };
	double angle = 0;
	sf::ConvexShape shape;
	sf::CircleShape circle;
	bool exist = 0;
public:
	Hitbox() = default;
	Hitbox(sf::Vector2f center) :
		center(center) {};

	sf::Vector2f getCenter();
	sf::Vector2f getCorner(short);
	double getAngle();
	bool getExist();

	void setCenter(sf::Vector2f);
	void setCenter(float, float);
	void setCorner(short, sf::Vector2f);
	void setCorner(short, float, float);
	void setAngle(double);
	void setExist(bool);

	void update();
	bool isCollided(Hitbox);
	bool isContained(Hitbox);
	void draw(sf::Color);
};