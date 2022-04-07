#include "Notice.h"

extern sf::RenderWindow window;
extern sf::Sprite noticeFrameSprite;
extern sf::Font fontEn;
extern Notice notices[3];
extern int noticeCount;

Notice::Notice(std::string message)
{
	this->message.setFont(fontEn);
	this->message.setFillColor(sf::Color::White);
	this->message.setCharacterSize(40);
	this->message.setString(message);
	this->message.setOrigin(this->message.getGlobalBounds().width / 2, this->message.getGlobalBounds().height / 2);
	this->exist = true;
	this->phase = 0;
	this->frames = 0;
	noticeCount++;
	if (noticeCount == 3)
	{
		noticeCount = 0;
	}
}
void Notice::setExist(bool x)
{
	this->exist = x;
}
void Notice::update()
{
	if (!this->exist)
	{
		return;
	}
	this->frames++;
	if (this->phase == 0)
	{
		this->coor.y = double(this->frames) / this->moveInTime * 50;
		if (this->frames == this->moveInTime)
		{
			this->frames = 0;
			this->phase++;
		}
	}
	else if (this->phase == 1)
	{
		if (this->frames == this->showTime)
		{
			this->frames = 0;
			this->phase++;
		}
	}
	else if (this->phase == 2)
	{
		this->coor.y = 50 - double(this->frames) / this->moveInTime * 50;
		if (this->frames == this->moveOutTime)
		{
			this->exist = false;
		}
	}
}
void Notice::draw()
{
	if (!this->exist)
	{
		return;
	}
	noticeFrameSprite.setPosition(window.getView().getCenter() + sf::Vector2f(-300, -410) + this->coor);
	window.draw(noticeFrameSprite);
	this->message.setPosition(window.getView().getCenter() + sf::Vector2f(0, -410) + this->coor);
	window.draw(this->message);
}