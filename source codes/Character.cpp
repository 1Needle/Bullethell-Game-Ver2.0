#include "Function.h"

#define pi 3.1415926

extern sf::Vector2i mapSize;
extern double radian(double);
extern double degree(double);
extern long long int frametime;
extern Bomb bombs[50];
extern Beam beams[20];
extern int enemyCount;
extern int bulletCount;
extern int animationCount;
extern int bombCount;
extern int beamCount;
extern int itemCount;
extern int score;
extern sf::RenderWindow window;
extern Bullet bullets[2000];
extern Animation animations[50];
extern Item items[50];
extern sf::Texture bulletHitTexture, enemyDieTexture;
extern sf::Sprite shipSprite[3], enemySprite[18];
extern Player playerShip;
extern Notice notices[3];
extern int noticeCount;
extern sf::Sound playerShoot, enemyShoot, playerHit, playerDie, reload, shieldBreak, powerUp;
extern sf::Music gameoverMusic, battleMusic;
extern bool inGame, lose;
extern sf::RectangleShape reloadBar[2];

Character::Character(double health, unsigned int sprite, unsigned int maxSprite, sf::Vector2f coor)
{
	this->health = health;
	this->sprite = sprite;
	this->maxSprite = maxSprite;
	this->coor = coor;
}
Character::Character(double health, unsigned int sprite, unsigned int maxSprite, sf::Vector2f coor,
	double maxSpeed, double minSpeed, double acceleration, double brake)
{
	this->health = health;
	this->sprite = sprite;
	this->maxSprite = maxSprite;
	this->coor = coor;
	this->maxSpeed = maxSpeed;
	this->minSpeed = minSpeed;
	this->acceleration = acceleration;
	this->brake = brake;
}
Character::Character(double health, double maxSpeed, sf::Vector2f coor)
{
	this->health = health;
	this->maxSpeed = maxSpeed;
	this->coor = coor;
}

double Character::getSpeed()
{
	return this->speed;
}
double Character::getMaxSpeed()
{
	return this->maxSpeed;
}
double Character::getMinSpeed()
{
	return this->minSpeed;
}
double Character::getHorizontalSpeed()
{
	return this->horizontalSpeed;
}
double Character::getVerticalSpeed()
{
	return this->verticalSpeed;
}
double Character::getAcceleration()
{
	return this->acceleration;
}
double Character::getAngle()
{
	return this->angle;
}
double Character::getDirection()
{
	return this->direction;
}
double Character::getHealth()
{
	return this->health;
}
double Character::getBrake()
{
	return this->brake;
}
unsigned int Character::getSprite()
{
	return this->sprite;
}
unsigned int Character::getMaxSprite()
{
	return this->maxSprite;
}
bool Character::getExist()
{
	return this->exist;
}
float Character::getX()
{
	return this->coor.x;
}
float Character::getY()
{
	return this->coor.y;
}
sf::Vector2f Character::getCoor()
{
	return this->coor;
}
Hitbox& Character::getHitbox(short x)
{
	Hitbox& ref = this->hitbox[x];
	return ref;
}

void Character::setSpeed(double x)
{
	this->speed = x;
}
void Character::setMaxSpeed(double x)
{
	this->maxSpeed = x;
}
void Character::setMinSpeed(double x)
{
	this->minSpeed = x;
}
void Character::setHorizontalSpeed(double x)
{
	this->horizontalSpeed = x;
}
void Character::setVerticalSpeed(double x)
{
	this->verticalSpeed = x;
}
void Character::setAcceleration(double x)
{
	this->acceleration = x;
}
void Character::setAngle(double x)
{
	this->angle = x;
}
void Character::setDirection(double x)
{
	this->direction = x;
}
void Character::setHealth(double x)
{
	this->health = x;
}
void Character::setBrake(double x)
{
	this->brake = x;
}
void Character::setSprite(unsigned int x)
{
	this->sprite = x;
}
void Character::setMaxSprite(unsigned int x)
{
	this->maxSprite = x;
}
void Character::setExist(bool x)
{
	this->exist = x;
}
void Character::setX(float x)
{
	this->coor.x = x;
}
void Character::setY(float x)
{
	this->coor.y = x;
}
void Character::setCoor(float x, float y)
{
	this->coor.x = x;
	this->coor.y = y;
}
void Character::setCoor(sf::Vector2f v)
{
	this->coor = v;
}

void Character::update()
{
	if (!this->getExist())
	{
		return;
	}
	//update sprite
	if (frametime % 20 == 0)
	{
		this->sprite += 1;
		if (this->sprite == this->maxSprite)
		{
			this->sprite = 0;
		}
	}
	// update position
	this->coor.x += float(this->horizontalSpeed);
	this->coor.y += float(this->verticalSpeed);
	if (this->coor.x < 0)
	{
		this->coor.x = 0;
	} 
	else if (this->coor.x > mapSize.x)
	{
		this->coor.x = float(mapSize.x);
	}
	if (this->coor.y < 0)
	{
		this->coor.y = 0;
	}
	else if (this->coor.y > mapSize.y)
	{
		this->coor.y = float(mapSize.y);
	}
	// update hitbox
	for (auto& x : this->hitbox)
	{
		if (!x.getExist())
		{
			return;
		}
		x.setCenter(this->coor);
		x.setAngle(this->angle);
		x.update();
	}
}
void Character::drawHitbox(sf::Color color)
{
	if (!this->exist)
	{
		return;
	}
	for (int i = 0; i < 4; i++)
	{
		this->getHitbox(i).draw(color);
	}
}
void Character::move(double angle)
{
	if (!this->getExist())
	{
		return;
	}
	if (angle == 200) // if not moving, then slow down
	{
		this->horizontalSpeed *= this->brake;
		this->verticalSpeed *= this->brake;
	}
	else // if moving, then speed up
	{
		this->horizontalSpeed += this->acceleration * cos(radian(angle));
		this->verticalSpeed += this->acceleration * sin(radian(angle));
	}
	this->speed = sqrt(pow(this->horizontalSpeed, 2) + pow(this->verticalSpeed, 2)); // update speed value
	if (this->speed > this->maxSpeed) // if too fast, then set to max speed
	{
		this->horizontalSpeed = this->maxSpeed * cos(radian(angle));
		this->verticalSpeed = this->maxSpeed * sin(radian(angle));
		this->speed = this->maxSpeed;
	}
	else if (this->speed < this->minSpeed) // if too slow, then stop it;
	{
		this->horizontalSpeed = 0;
		this->verticalSpeed = 0;
		this->speed = 0;
	}
}

Player::Player(double health, unsigned int sprite, unsigned int maxSprite, sf::Vector2f coor,
	double maxSpeed, double minSpeed, double acceleration, double brake)
{
	this->Character::Character(health, sprite, maxSprite, coor, maxSpeed, minSpeed, acceleration, brake);
	this->setExist(1);
}

short Player::getLevel()
{
	return this->level;
}
double Player::getPower()
{
	return this->power;
}
double Player::getMaxHealth()
{
	return this->maxHealth;
}
double Player::getMaxPower()
{
	return this->maxPower;
}
int Player::getAmmo()
{
	return this->ammo;
}
int Player::getMaxAmmo()
{
	return this->maxAmmo;
}
int Player::getBomb()
{
	return this->bomb;
}
int Player::getMaxBomb()
{
	return this->maxBomb;
}
int Player::getReloadTime()
{
	return this->reloadTime;
}
int Player::getMaxReloadTime()
{
	return this->maxReloadTime;
}
bool Player::getShield()
{
	return this->shield;
}
bool Player::getShieldBlur()
{
	return this->shieldBlur;
}
bool Player::getInvincible()
{
	return this->invincible;
}
bool Player::getReloading()
{
	return this->reloading;
}

void Player::setPower(double x)
{
	this->power = x;
	int preLevel = this->level;
	if (this->power >= 20)
	{
		this->level = 2;
	}
	else if (this->power >= 10)
	{
		this->level = 1;
	}
	else
	{
		this->level = 0;
	}
	if (preLevel != this->level)
	{
		powerUp.play();
		notices[noticeCount] = Notice("Power Level Up!!!");
		updateLevel();
	}
}

void Player::setAmmo(int x)
{
	this->ammo = x;
}
void Player::setMaxAmmo(int x)
{
	this->maxAmmo = x;
}
void Player::setBomb(int x)
{
	this->bomb = x;
}
void Player::setReloadTime(int x)
{
	this->reloadTime = x;
}
void Player::setMaxReloadTime(int x)
{
	this->maxReloadTime = x;
}
void Player::setShield(bool x)
{
	if (x == true)
	{
		this->shield = true;
		this->shieldTime = 0;
	}
	else
	{
		this->shieldTime = this->shieldMaxTime - 1;
	}
}
void Player::setInvincible(bool x)
{
	this->invincible = x;
}
void Player::setReloading(bool x)
{
	this->reloading = x;
}

void Player::update()
{
	if (!this->getExist())
	{
		return;
	}
	// handling movements and sprite
	this->Character::update();
	// handling shooting Cooldown
	if (this->shootCooldown != 0)
	{
		shootCooldown--;
	}
	if (this->bombCooldown != 0)
	{
		bombCooldown--;
	}
	this->setAngle(faceToward(this->getCoor(), sf::Vector2f(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) - 180);
	// handling shield exist time
	if (this->shield)
	{
		this->shieldTime++;
		if (this->shieldTime == this->shieldMaxTime)
		{
			shieldBreak.play();
			animations[animationCount] = Animation("shieldBreak", this->getCoor());
			animations[animationCount] = Animation("shieldBreak", this->getCoor(), 10);
			this->shieldTime = 0;
			this->shield = false;
		}
		if (this->shieldMaxTime - this->shieldTime <= 180 && this->shieldTime % 40 < 20)
		{
			this->shieldBlur = true;
		}
		else
		{
			this->shieldBlur = false;
		}
	}
	// handling invincible timing
	if (this->invincible)
	{
		invincibleTime++;
		if (invincibleTime % 30 < 15)
		{
			this->invincibleBlur = true;
		}
		else
		{
			this->invincibleBlur = false;
		}
		if (this->invincibleTime == this->invincibleMaxTime)
		{
			this->invincible = false;
			this->invincibleBlur = false;
			this->invincibleTime = 0;
		}
	}
	// reload
	if (this->reloading)
	{
		if (this->reloadTime % 20 == 0)
		{
			reload.play();
		}
		playerShip.reloadTime++;
		double percent = double(playerShip.reloadTime) / playerShip.maxReloadTime;
		if (percent == 1)
		{
			this->reloading = 0;
			playerShip.ammo = playerShip.maxAmmo;
			playerShip.reloadTime = 0;
		}
		reloadBar[1].setPosition(playerShip.getCoor() + sf::Vector2f(-40, 35));
		reloadBar[1].setSize(sf::Vector2f(80 * float(percent), 9));
		reloadBar[0].setPosition(playerShip.getCoor() + sf::Vector2f(-40 + float(80 * percent), 35));
		reloadBar[0].setSize(sf::Vector2f(80 * float(1 - percent), 9));
	}
}
void Player::updateLevel()
{
	switch (this->level)
	{
	default:
		break;
	case 2:
		this->maxAmmo = 30;
		this->ammo = 30;
		this->getHitbox(0).setCorner(0, -39, -6);
		this->getHitbox(0).setCorner(1, 39, -6);
		this->getHitbox(0).setCorner(2, 39, 9);
		this->getHitbox(0).setCorner(3, -39, 9);
		this->getHitbox(0).setExist(1);
		this->getHitbox(1).setCorner(0, -6, -27);
		this->getHitbox(1).setCorner(1, 6, -27);
		this->getHitbox(1).setCorner(2, 6, 27);
		this->getHitbox(1).setCorner(3, -6, 27);
		this->getHitbox(1).setExist(1);
		break;
	case 1:
		this->maxAmmo = 25;
		this->ammo = 25;
		this->getHitbox(0).setCorner(0, -39, -15);
		this->getHitbox(0).setCorner(1, 39, -15);
		this->getHitbox(0).setCorner(2, 39, 0);
		this->getHitbox(0).setCorner(3, -39, 0);
		this->getHitbox(0).setExist(1);
		this->getHitbox(1).setCorner(0, -6, -21);
		this->getHitbox(1).setCorner(1, 6, -21);
		this->getHitbox(1).setCorner(2, 6, 21);
		this->getHitbox(1).setCorner(3, -6, 21);
		this->getHitbox(1).setExist(1);
		break;
	case 0:
		this->maxAmmo = 20;
		this->ammo = 20;
		this->getHitbox(0).setCorner(0, -24, -9);
		this->getHitbox(0).setCorner(1, 21, -9);
		this->getHitbox(0).setCorner(2, 21, 3);
		this->getHitbox(0).setCorner(3, -24, 3);
		this->getHitbox(0).setExist(1);
		this->getHitbox(1).setCorner(0, -6, -15);
		this->getHitbox(1).setCorner(1, 6, -15);
		this->getHitbox(1).setCorner(2, 6, 18);
		this->getHitbox(1).setCorner(3, -6, 18);
		this->getHitbox(1).setExist(1);
		break;
	}
}
void Player::draw()
{
	if (!this->getExist())
	{
		return;
	}
	if (this->invincibleBlur == true)
	{
		shipSprite[playerShip.getLevel()].setColor(sf::Color(150, 150, 150, 128));
	}
	else if (this->invincibleBlur == false)
	{
		shipSprite[playerShip.getLevel()].setColor(sf::Color(255, 255, 255, 255));
	}
	shipSprite[playerShip.getLevel()].setPosition(playerShip.getCoor());
	shipSprite[playerShip.getLevel()].setRotation(float(playerShip.getAngle()));
	window.draw(shipSprite[playerShip.getLevel()]);
	if (reloading)
	{
		window.draw(reloadBar[1]);
		window.draw(reloadBar[0]);
	}
}
void Player::shoot()
{
	if (this->shootCooldown == 0) // if not in shootCooldown
	{
		playerShoot.play();
		switch (this->level)
		{
		case 2:
			bullets[bulletCount] = Bullet("level3", this->getCoor(), -90 + this->getAngle());
			bullets[bulletCount] = Bullet("level3", this->getCoor(), -95 + this->getAngle());
			bullets[bulletCount] = Bullet("level3", this->getCoor(), -85 + this->getAngle());
			bullets[bulletCount] = Bullet("level3", this->getCoor(), -100 + this->getAngle());
			bullets[bulletCount] = Bullet("level3", this->getCoor(), -80 + this->getAngle());
			if (this->ammo % 3 == 0)
			{
				bullets[bulletCount] = Bullet("homing", this->getCoor(), -90 + this->getAngle());
				bullets[bulletCount] = Bullet("homing", this->getCoor(), -60 + this->getAngle());
				bullets[bulletCount] = Bullet("homing", this->getCoor(), -80 + this->getAngle());
				bullets[bulletCount] = Bullet("homing", this->getCoor(), -100 + this->getAngle());
				bullets[bulletCount] = Bullet("homing", this->getCoor(), -120 + this->getAngle());
			}
			this->shootCooldown = 5;
			break;
		case 1:
			bullets[bulletCount] = Bullet("level2", this->getCoor(), -90 + this->getAngle());
			bullets[bulletCount] = Bullet("level2", this->getCoor(), -95 + this->getAngle());
			bullets[bulletCount] = Bullet("level2", this->getCoor(), -85 + this->getAngle());
			if (this->ammo % 3 == 0)
			{
				bullets[bulletCount] = Bullet("homing", this->getCoor(), -90 + this->getAngle());
				bullets[bulletCount] = Bullet("homing", this->getCoor(), -80 + this->getAngle());
				bullets[bulletCount] = Bullet("homing", this->getCoor(), -100 + this->getAngle());
			}
			this->shootCooldown = 7;
			break;
		case 0:
			bullets[bulletCount] = Bullet("level1", this->getCoor(), -90 + this->getAngle());
			if (this->ammo % 3 == 0)
			{
				bullets[bulletCount] = Bullet("homing", this->getCoor(), -80 + this->getAngle());
				bullets[bulletCount] = Bullet("homing", this->getCoor(), -100 + this->getAngle());
			}
			this->shootCooldown = 10;
			break;
		default:
			break;
		}
		this->ammo--;
	}
}
void Player::fireBomb()
{
	if (this->bombCooldown == 0)
	{
		switch (this->level)
		{
		case 2:
			bombs[bombCount] = Bomb("level3", this->getCoor(), this->getAngle() - 90);
			break;
		case 1:
			bombs[bombCount] = Bomb("level2", this->getCoor(), this->getAngle() - 90);
			break;
		case 0:
			bombs[bombCount] = Bomb("level1", this->getCoor(), this->getAngle() - 90);
			break;
		}
		this->bombCooldown = 300;
		this->bomb--;
	}
}

Enemy::Enemy(std::string type, sf::Vector2f coor)
{
	this->damage = 10;
	this->type = type;
	this->setCoor(coor);
	this->initialize();
}

void Enemy::initialize()
{
	if (this->type == "shotgun")
	{
		this->setHealth(100);
		this->maxHealth = 100;
		this->setMaxSpeed(2);
		this->setAcceleration(2);
		this->setBrake(0.7);
		this->setMaxSprite(2);
		this->getHitbox(0).setCorner(0, -30, -36);
		this->getHitbox(0).setCorner(1, 30, -36);
		this->getHitbox(0).setCorner(2, 30, 36);
		this->getHitbox(0).setCorner(3, -30, 36);
		this->getHitbox(0).setExist(1);
	}
	else if (this->type == "beam")
	{
		this->setHealth(80);
		this->maxHealth = 80;
		this->setMaxSpeed(2);
		this->setAcceleration(1);
		this->setBrake(0.7);
		this->setMaxSprite(2);
		this->getHitbox(0).setCorner(0, -30, -36);
		this->getHitbox(0).setCorner(1, 30, -36);
		this->getHitbox(0).setCorner(2, 30, 36);
		this->getHitbox(0).setCorner(3, -30, 36);
		this->getHitbox(0).setExist(1);
	}
	else if (this->type == "guerilla")
	{
		this->setHealth(120);
		this->maxHealth = 120;
		this->setMaxSpeed(6);
		this->setAcceleration(2);
		this->setBrake(0.7);
		this->setMaxSprite(2);
		this->getHitbox(0).setCorner(0, -30, -36);
		this->getHitbox(0).setCorner(1, 30, -36);
		this->getHitbox(0).setCorner(2, 30, 36);
		this->getHitbox(0).setCorner(3, -30, 36);
		this->getHitbox(0).setExist(1);
	}
	else if (this->type == "tank")
	{
		this->setHealth(300);
		this->maxHealth = 300;
		this->setMaxSpeed(1);
		this->setAcceleration(1.2);
		this->setBrake(0.7);
		this->setMaxSprite(2);
		this->getHitbox(0).setCorner(0, -30, -36);
		this->getHitbox(0).setCorner(1, 30, -36);
		this->getHitbox(0).setCorner(2, 30, 36);
		this->getHitbox(0).setCorner(3, -30, 36);
		this->getHitbox(0).setExist(1);
	}
	else if (this->type == "madman")
	{
		this->setHealth(100);
		this->maxHealth = 100;
		this->setMaxSpeed(2.5);
		this->setAcceleration(2);
		this->setBrake(0.7);
		this->setMaxSprite(2);
		this->getHitbox(0).setCorner(0, -30, -36);
		this->getHitbox(0).setCorner(1, 30, -36);
		this->getHitbox(0).setCorner(2, 30, 36);
		this->getHitbox(0).setCorner(3, -30, 36);
		this->getHitbox(0).setExist(1);
	}
	else if (this->type == "charger")
	{
		this->setHealth(10);
		this->maxHealth = 10;
		this->setMaxSpeed(30);
		this->setAcceleration(15);
		this->setBrake(0.7);
		this->setMaxSprite(2);
		this->getHitbox(0).setCorner(0, -18, -21);
		this->getHitbox(0).setCorner(1, 15, -21);
		this->getHitbox(0).setCorner(2, 15, 24);
		this->getHitbox(0).setCorner(3, -18, 24);
		this->getHitbox(0).setExist(1);
	}
	else if (this->type == "barrier")
	{
		this->setHealth(300);
		this->maxHealth = 300;
		this->setMaxSpeed(0);
		this->setAcceleration(2);
		this->setBrake(0.7);
		this->setMaxSprite(2);
		this->getHitbox(0).setCorner(0, -27, -27);
		this->getHitbox(0).setCorner(1, 27, -27);
		this->getHitbox(0).setCorner(2, 27, 27);
		this->getHitbox(0).setCorner(3, -27, 27);
		this->getHitbox(0).setExist(1);
	}
	else if (this->type == "battery")
	{
		this->setHealth(100);
		this->maxHealth = 100;
		this->setMaxSpeed(0);
		this->setAcceleration(2);
		this->setBrake(0.7);
		this->setMaxSprite(1);
		this->getHitbox(0).setCorner(0, -36, -36);
		this->getHitbox(0).setCorner(1, 36, -36);
		this->getHitbox(0).setCorner(2, 36, 36);
		this->getHitbox(0).setCorner(3, -36, 36);
		this->getHitbox(0).setExist(1);
	}
	else if (this->type == "mage")
	{
		this->setHealth(30);
		this->maxHealth = 30;
		this->setMaxSpeed(10);
		this->setAcceleration(2);
		this->setBrake(0.7);
		this->setMaxSprite(3);
		this->getHitbox(0).setCorner(0, -36, -24);
		this->getHitbox(0).setCorner(1, 36, -24);
		this->getHitbox(0).setCorner(2, 36, 27);
		this->getHitbox(0).setCorner(3, -36, 27);
		this->getHitbox(0).setExist(1);
	}
	this->setExist(1);
	this->setMinSpeed(1);
	enemyCount++;
	if (enemyCount == 100)
	{
		enemyCount = 0;
	}
}

std::string Enemy::getType()
{
	return this->type;
}

void Enemy::moveset()
{
	double angle = faceToward(this->getCoor(), playerShip.getCoor()) + 90;
	sf::Vector2f difference = this->getCoor() - playerShip.getCoor();
	double distance = sqrt(difference.x * difference.x + difference.y * difference.y);
	if (this->type == "shotgun")
	{
		this->move(angle);
		if (this->shootCooldown == 0 && distance < 300)
		{
			this->shoot("greenMagic", angle - 10);
			this->shoot("greenMagic", angle + 10);
			this->shoot("greenMagic", angle - 30);
			this->shoot("greenMagic", angle + 30);
			this->shootCooldown = 90;
		}
	}
	else if (this->type == "beam")
	{
		if (this->moveCooldown == 0)
		{
			if (distance >= 600)
			{
				this->move(angle);
			}
			else if (distance <= 500)
			{
				this->move(angle - 180);
			}
			else
			{
				if (rand() % 2 == 0)
				{
					this->move(angle - 90);
				}
				else
				{
					this->move(angle + 90);
				}
				this->moveCooldown = 30;
			}
		}
		if (distance < 700)
		{
			if (this->shootCooldown == 0)
			{
				this->setHorizontalSpeed(0);
				this->setVerticalSpeed(0);
				this->shootBeam(angle);
				shootCooldown = 300;
				moveCooldown = 90;
			}
		}
	}
	else if (this->type == "guerilla")
	{
		if (distance <= 300)//move away from player
		{
			this->move(angle - 180);
		}
		else if (distance >= 400)//approach toward player
		{
			this->move(angle);
		}
		else if (this->moveCooldown == 0)
		{
			if (rand() % 2 == 0)
			{
				this->move(angle - 90);
			}
			else
			{
				this->move(angle + 90);
			}
			this->moveCooldown = 60;
		}
		if (this->shootCooldown == 0 && distance < 400)
		{
			this->shoot("redCone", angle - 15 + rand() % 30);
			this->shootCooldown = 30;
		}
	}
	else if (this->type == "tank")
	{
		this->move(angle);
	}
	else if (this->type == "madman")
	{
		move(angle);
		if (this->shootCooldown == 0)
		{
			this->shoot("yellowCone", rand() % 360);
			this->shootCooldown = 7;
		}
	}
	else if (this->type == "charger")
	{
		if (this->moveCooldown > 240)
		{
			this->pause = true;
		}
		else
		{
			pause = false;
		}
		if (this->pause == false)
		{
			this->setAngle(angle + 90);
			if (distance <= 400 && this->moveCooldown == 0)
			{
				this->setMaxSpeed(30);
				this->move(angle);
				this->moveCooldown = 300;
			}
			else if (distance >= 500 && this->moveCooldown == 0)
			{
				this->setMaxSpeed(2);
				this->move(angle);
			}
			else if (this->moveCooldown != 0)
			{
				this->setMaxSpeed(1);
				this->move(angle - 180);
			}
		}
	}
	else if (this->type == "barrier")
	{
		
	}
	else if (this->type == "battery")
	{
		if (shootCooldown == 0)
		{
			for (int i = 0; i < 10; i++)
			{
				this->shoot("purpleBall", angle + i * 36);
			}
			shootCooldown = 300;
		}
		else if (shootCooldown == 280)
		{
			for (int i = 0; i < 10; i++)
			{
				this->shoot("pinkBall", angle + i * 36 + 18);
			}
		}
	}
	else if (this->type == "mage")
	{
	if (distance <= 400)//move away from player
	{
		this->move(angle - 120);
	}
	else if (distance >= 500)//approach toward player
	{
		this->move(angle);
	}
	else if (this->moveCooldown == 0)
	{
		if (rand() % 2 == 0)
		{
			this->move(angle - 90);
		}
		else
		{
			this->move(angle + 90);
		}
		this->moveCooldown = 60;
	}
	if (this->shootCooldown == 0)
	{
		for (int i = 0; i < 10; i++)
		{
			this->shoot("blueMagic", i * 36);
		}
		this->shootCooldown = 150;
	}
	}
	if (this->shootCooldown != 0)
	{
		this->shootCooldown--;
	}
	if (this->moveCooldown != 0)
	{
		this->moveCooldown--;
	}
}
void Enemy::update()
{
	if (!this->getExist())
	{
		return;
	}
	this->moveset();
	this->Character::update();
	if (!playerShip.getInvincible())
	{
		if (isCollided(*this, playerShip))
		{
			if (playerShip.getInvincible() == false)
			{
				if (playerShip.getShield())
				{
					shieldBreak.play();
					playerShip.setShield(false);
					playerShip.setInvincible(true);
				}
				else
				{
					playerHit.play();
					playerShip.setHealth(playerShip.getHealth() - this->damage);
					playerShip.setInvincible(true);
					if (playerShip.getHealth() <= 0)
					{
						playerDie.play();
						playerShip.setExist(false);
						inGame = false;
						lose = true;
						gameoverMusic.play();
						battleMusic.stop();
					}
				}
			}
		}
	}
}
void Enemy::draw()
{
	if (!this->getExist())
	{
		return;
	}
	if (this->type == "shotgun")
	{
		enemySprite[0 + this->getSprite()].setPosition(this->getCoor());
		window.draw(enemySprite[0 + this->getSprite()]);
	}
	else if (this->type == "beam")
	{
		enemySprite[2 + this->getSprite()].setPosition(this->getCoor());
		window.draw(enemySprite[2 + this->getSprite()]);
	}
	else if (this->type == "guerilla")
	{
		enemySprite[4 + this->getSprite()].setPosition(this->getCoor());
		window.draw(enemySprite[4 + this->getSprite()]);
	}
	else if (this->type == "tank")
	{
		enemySprite[6 + this->getSprite()].setPosition(this->getCoor());
		window.draw(enemySprite[6 + this->getSprite()]);
	}
	else if (this->type == "madman")
	{
		enemySprite[8 + this->getSprite()].setPosition(this->getCoor());
		window.draw(enemySprite[8 + this->getSprite()]);
	}
	else if (this->type == "charger")
	{
		enemySprite[10 + this->getSprite()].setRotation(this->getAngle());
		enemySprite[10 + this->getSprite()].setPosition(this->getCoor());
		window.draw(enemySprite[10 + this->getSprite()]);
	}
	else if (this->type == "barrier")
	{
		enemySprite[12 + this->getSprite()].setPosition(this->getCoor());
		window.draw(enemySprite[12 + this->getSprite()]);
	}
	else if (this->type == "battery")
	{
		enemySprite[14].setPosition(this->getCoor());
		window.draw(enemySprite[14]);
	}
	else if (this->type == "mage")
	{
		enemySprite[15 + this->getSprite()].setPosition(this->getCoor());
		window.draw(enemySprite[15 + this->getSprite()]);
	}
	this->drawHealthBar();
}
void Enemy::shoot(std::string type, double direction)
{
	bullets[bulletCount] = Bullet(type, this->getCoor(), direction);
	enemyShoot.play();
}
void Enemy::shootBeam(double direction)
{
	beams[beamCount] = Beam(this->getCoor(), direction);
}
void Enemy::drawHealthBar()
{
	if (this->getHealth() == this->maxHealth)
	{
		return;
	}
	double percent = this->getHealth() / this->maxHealth;
	sf::RectangleShape red, gray;
	double width = this->getHitbox(0).getCorner(1).x - this->getHitbox(0).getCorner(0).x;
	double height = 9;
	sf::Vector2f coor = this->getHitbox(0).getCenter() + this->getHitbox(0).getCorner(3);
	red.setFillColor(sf::Color::Red);
	red.setSize(sf::Vector2f(float(width * percent), float(height)));
	red.setPosition(coor.x, coor.y + 3);
	window.draw(red);

	gray.setFillColor(sf::Color(128, 128, 128, 255));
	gray.setSize(sf::Vector2f(float(width * (1 - percent)), float(height)));
	gray.setPosition(float(coor.x + width * percent), coor.y + 3);
	window.draw(gray);
}