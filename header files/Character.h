#pragma once
#include <SFML/Graphics.hpp>
#include "Hitbox.h"

class Character
{
private:
	double speed = 0, maxSpeed = 0, minSpeed = 0;
	double horizontalSpeed = 0, verticalSpeed = 0;
	double acceleration = 0;
	double angle = 0, direction = 0;
	double health = 0;
	double brake = 0;
	unsigned int sprite = 0;
	unsigned int maxSprite = 0;
	bool exist = 0;
	sf::Vector2f coor = sf::Vector2f(0, 0);
	Hitbox hitbox[4];
public:

	Character() = default;
	Character(double health, unsigned int sprite, unsigned int maxSprite, sf::Vector2f coor);
	Character(double health, unsigned int sprite, unsigned int maxSprite, sf::Vector2f coor,
		double maxSpeed, double minSpeed, double acceleration, double brake);
	Character(double health, double maxSpeed, sf::Vector2f coor);

	double getSpeed();
	double getMaxSpeed();
	double getMinSpeed();
	double getHorizontalSpeed();
	double getVerticalSpeed();
	double getAcceleration();
	double getAngle();
	double getDirection();
	double getHealth();
	double getBrake();
	unsigned int getSprite();
	unsigned int getMaxSprite();
	bool getExist();
	float getX();
	float getY();
	sf::Vector2f getCoor();
	Hitbox& getHitbox(short);

	void setSpeed(double);
	void setMaxSpeed(double);
	void setMinSpeed(double);
	void setHorizontalSpeed(double);
	void setVerticalSpeed(double);
	void setAcceleration(double);
	void setAngle(double);
	void setDirection(double);
	void setHealth(double);
	void setBrake(double);
	void setSprite(unsigned int);
	void setMaxSprite(unsigned int);
	void setExist(bool);
	void setX(float);
	void setY(float);
	void setCoor(float, float);
	void setCoor(sf::Vector2f);

	void update();
	void drawHitbox(sf::Color);
	void move(double);
};

class Player : public Character
{
private:
	double power = 0;
	short level = 0;
	short shootCooldown = 0;
	short bombCooldown = 0;
	double maxHealth = 100;
	double maxPower = 20;
	int ammo = 0;
	int maxAmmo = 0;
	int bomb = 3;
	int maxBomb = 10;
	int reloadTime = 0;
	int maxReloadTime = 45;
	bool reloading = 0;
	bool shield = 0;
	bool shieldBlur = 0;
	int shieldMaxTime = 600;
	int shieldTime = 0;
	bool invincible = 0;
	bool invincibleBlur = 0;
	int invincibleMaxTime = 90;
	int invincibleTime = 0;
public:
	Player(double health, unsigned int sprite, unsigned int maxSprite, sf::Vector2f coor,
		double maxSpeed, double minSpeed, double acceleration, double brake);

	short getLevel();
	double getPower();
	double getMaxHealth();
	double getMaxPower();
	int getAmmo();
	int getMaxAmmo();
	int getBomb();
	int getMaxBomb();
	int getReloadTime();
	int getMaxReloadTime();
	bool getShield();
	bool getShieldBlur();
	bool getInvincible();
	bool getReloading();

	void setPower(double);
	void setAmmo(int);
	void setMaxAmmo(int);
	void setBomb(int);
	void setReloadTime(int);
	void setMaxReloadTime(int);
	void setShield(bool);
	void setInvincible(bool);
	void setReloading(bool);

	void update();
	void updateLevel();
	void draw();
	void shoot();
	void fireBomb();
};

class Enemy : public Character
{
private:
	std::string type;
	double maxHealth = 0;
	double damage = 0;
	int shootCooldown = 0;
	int moveCooldown = 0;
	bool pause = 0;
	void initialize();
public:
	Enemy() = default;
	Enemy(std::string type, sf::Vector2f coor);

	std::string getType();

	void moveset();
	void update();
	void draw();
	void shoot(std::string type, double direction);
	void shootBeam(double direction);
	void drawHealthBar();
};