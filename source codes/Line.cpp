#include <SFML/Graphics.hpp>
#include "Line.h"

extern sf::RenderWindow window;

sf::Vector2f Line::getDot(short x)
{
	return this->dot[x];
}
double Line::getA()
{
	return this->a;
}
double Line::getB()
{
	return this->b;
}
double Line::getC()
{
	return this->c;
}

void Line::setDot(short x, sf::Vector2f y)
{
	this->dot[x] = y;
}
void Line::setDot(short x, float y, float z)
{
	this->dot[x] = sf::Vector2f(y, z);
}

void Line::update()
{
	// calculate the formation ax + by = c
	this->a = double(this->dot[0].y - this->dot[1].y);
	this->b = double(this->dot[1].x - this->dot[0].x);
	if (this->a == 0)
	{
		this->b = 1;
		this->c = this->getDot(0).y;
	}
	else if (this->b == 0)
	{
		this->a = 1;
		this->c = this->getDot(0).x;
	}
	else
	{
		this->c = double(this->dot[1].x * this->dot[0].y - this->dot[0].x * this->dot[1].y);
	}
}
bool Line::isIntersect(Line l)
{
	double delta = (this->a * l.b) - (this->b * l.a);
	if (delta == 0)
	{
		if (((this->a * l.b) == (this->b * l.a)) && ((this->b * l.c) == (this->c * l.b)) && ((this->a * l.c) == (this->c * l.a)))
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		double x = ((this->c * l.b) - (this->b * l.c)) / delta;
		double y = ((this->a * l.c) - (this->c * l.a)) / delta;
		if (((this->dot[0].x <= x) && (this->dot[1].x >= x)) || ((this->dot[0].x >= x) && (this->dot[1].x <= x)))
		{
			if (((l.dot[0].x <= x) && (l.dot[1].x >= x)) || ((l.dot[0].x >= x) && (l.dot[1].x <= x)))
			{
				if (((this->dot[0].y <= y) && (this->dot[1].y >= y)) || ((this->dot[0].y >= y) && (this->dot[1].y <= y)))
				{
					if (((l.dot[0].y <= y) && (l.dot[1].y >= y)) || ((l.dot[0].y >= y) && (l.dot[1].y <= y)))
					{
						return 1;
					}
				}
			}
		}
		return 0;
	}
}
void Line::draw(sf::Color color)
{
	sf::VertexArray l(sf::Lines, 2);
	l[0].position = this->dot[0];
	l[1].position = this->dot[1];
	l[0].color = color;
	l[1].color = color;
	window.draw(l);
}