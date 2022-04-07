#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Animation
{
private:
	int frame = 0;
	int maxFrame = 0;
	int index = 0;
	int maxIndex = 0;
	bool repeat = 0;
	sf::Vector2f coor = sf::Vector2f(0, 0);
	bool exist = 0;
	int delay = 0;
	std::string type;

	void initialize();
public:
	Animation() = default;
	Animation(std::string type, sf::Vector2f coor);
	Animation(std::string type, sf::Vector2f coor, int delay);

	void setCoor(float, float);
	void setCoor(sf::Vector2f);
	void setExist(bool);
	
	void update();
	void draw();
};