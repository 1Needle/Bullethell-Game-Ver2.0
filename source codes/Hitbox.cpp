#include "Hitbox.h"
#include "Function.h"
#include "Line.h"
#include <math.h>

using namespace std;

extern sf::RenderWindow window;

sf::Vector2f Hitbox::getCenter()
{
	return this->center;
}
sf::Vector2f Hitbox::getCorner(short x)
{
	return this->corner[x];
}
double Hitbox::getAngle()
{
	return this->angle;
}
bool Hitbox::getExist()
{
	return this->exist;
}

void Hitbox::setCenter(sf::Vector2f x)
{
	this->center = x;
}
void Hitbox::setCenter(float x, float y)
{
	this->center = sf::Vector2f(x, y);
}
void Hitbox::setCorner(short x, sf::Vector2f y)
{
	this->corner[x] = y;
}
void Hitbox::setCorner(short x, float y, float z)
{
	this->corner[x] = sf::Vector2f(y, z);
}
void Hitbox::setAngle(double x)
{
	this->angle = x;
}
void Hitbox::setExist(bool x)
{
	this->exist = x;
}

void Hitbox::update()
{
	if (!exist)
	{
		return;
	}
	this->shape.setPointCount(4);
	double cosA = cos(radian(this->angle));
	double sinA = sin(radian(this->angle));
	for (int i = 0; i < 4; i++)
	{
		sf::Vector2f corner;
		corner.x = float(this->corner[i].x * cosA + this->corner[i].y * (-sinA));
		corner.y = float(this->corner[i].x * sinA + this->corner[i].y * cosA);
		this->shape.setPoint(i, corner + this->center);
	}
}
bool Hitbox::isCollided(Hitbox box)
{
	if (this->shape.getGlobalBounds().intersects(box.shape.getGlobalBounds()))
	{
		if (this->isContained(box) || box.isContained(*this))
		{
			return 1;
		}
		Line line[8];
		for (int i = 0; i < 4; i++)
		{
			line[i].setDot(0, this->shape.getPoint(i));
			line[i].setDot(1, this->shape.getPoint((i + 1) * (i != 3)));
			line[i].update();
			line[i + 4].setDot(0, box.shape.getPoint(i));
			line[i + 4].setDot(1, box.shape.getPoint((i + 1) * (i != 3)));
			line[i + 4].update();
		}
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (line[i].isIntersect(line[4 + j]))
				{
					return 1;
				}
			}
		}
	}
	return 0;
}
bool Hitbox::isContained(Hitbox box)
{
	Line line[4];
	for (int i = 0; i < 4; i++)
	{
		line[i].setDot(0, this->shape.getPoint(i));
		line[i].setDot(1, this->shape.getPoint((i + 1) * (i != 3)));
		line[i].update();
	}
	double max1 = line[0].getA() * line[0].getDot(0).x + line[0].getB() * line[0].getDot(0).y;
	double min1 = line[0].getA() * line[2].getDot(0).x + line[0].getB() * line[2].getDot(0).y;
	if (max1 < min1)
	{
		double temp = max1;
		max1 = min1;
		min1 = temp;
	}
	double max2 = line[1].getA() * line[1].getDot(0).x + line[1].getB() * line[1].getDot(0).y;
	double min2 = line[1].getA() * line[3].getDot(0).x + line[1].getB() * line[3].getDot(0).y;
	if (max2 < min2)
	{
		double temp = max2;
		max2 = min2;
		min2 = temp;
	}
	for (int i = 0; i < 4; i++)
	{
		sf::Vector2f coor = box.shape.getPoint(i);
		bool pass[2] = { 0, 0 };
		double value1 = line[0].getA() * coor.x + line[0].getB() * coor.y;
		double value2 = line[1].getA() * coor.x + line[1].getB() * coor.y;
		if (!((value1 <= max1 && value1 >= min1) &&
			(value2 <= max2 && value2 >= min2)))
		{
			return 0;
		}
	}
	return 1;
}
void Hitbox::draw(sf::Color color)
{
	if (!exist)
	{
		return;
	}
	this->circle.setFillColor(color);
	this->circle.setRadius(3);
	this->circle.setOrigin(this->circle.getGlobalBounds().width / 2, this->circle.getGlobalBounds().height / 2);
	this->circle.setPosition(this->center);
	window.draw(circle);
	this->shape.setOutlineThickness(1);
	this->shape.setOutlineColor(color);
	this->shape.setFillColor(sf::Color::Transparent);
	window.draw(shape);
}