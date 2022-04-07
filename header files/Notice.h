#pragma once
#include "Function.h"

class Notice
{
private:
	sf::Text message;
	int showTime = 120;
	int moveInTime = 30;
	int moveOutTime = 30;
	int phase = 0;
	int frames = 0;
	bool exist = 0;
	sf::Vector2f coor = sf::Vector2f(0, 0);
public:
	Notice() = default;
	Notice(std::string message);

	void setExist(bool);

	void update();
	void draw();
};