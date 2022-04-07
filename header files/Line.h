#pragma once
#include <SFML/Graphics.hpp>

class Line
{
private:
	sf::Vector2f dot[2];
	double a = 0, b = 0, c = 0;
public:
	Line() = default;

	sf::Vector2f getDot(short);
	double getA();
	double getB();
	double getC();

	void setDot(short, sf::Vector2f);
	void setDot(short, float, float);

	void update();
	bool isIntersect(Line);
	void draw(sf::Color);
};