#include "Function.h"

using std::cout;
using std::endl;

#define pi 3.1415926

sf::Vector2i mapSize(2560, 1440);
sf::Vector2i uiSize(48 * 5, 48 * 3);
extern sf::RenderWindow window;
Stage stage;
Notice notices[3];
int noticeCount;
extern sf::RectangleShape spaceDustSprite[3];
extern sf::Texture shipTexture, tileTexture, characterTexture;
sf::Texture bulletHitTexture, enemyDieTexture, uiTexture, iconTexture, itemTexture, bulletTexture, selectCursorTexture,
				SFMLTexture, MODTexture, kenneyTexture;
extern sf::Sprite shipSprite[3], bulletSprite[25], enemySprite[18];
sf::Sprite bombSprite[4], mouseSprite, itemSprite[4], selectCursorSprite;
extern Player playerShip;
extern Enemy enemies[100];
Animation animations[50];
Item items[50];
extern Bullet bullets[2000];
Bomb bombs[50];
Beam beams[20];
extern int enemyCount;
extern int bulletCount;
int bombCount = 0;
int beamCount = 0;
int animationCount = 0;
int itemCount = 0;
int score = 0;
int timeSec = 0;
extern sf::Font fontEn, fontCh;
extern sf::Text debugText;
sf::Text scoreText, timeText, stageText;
extern long long int frametime;
extern bool debugMode;
std::stringstream ss;
sf::View view(sf::FloatRect(0, 0, 1280, 720)), minimap(sf::FloatRect(-10, -10, mapSize.x + 20.f, mapSize.y + 20.f));
sf::RenderTexture mapTexture, uiRenderTexture, scorePanelTexture, shieldTexture, beamTexture, noticeFrameTexture, warningFrameTexture;
sf::Sprite mapSprite, uiSprite, scorePanelSprite, ammoSprite[2], bombAmmoSprite[2], shieldSprite, bulletHitSprite, bombHitSprite,
			enemyDieSprite, shieldBreakSprite, beamSprite, beamStartupSprite, noticeFrameSprite, warningFrameSprite, SFMLSprite,
			MODSprite, kenneySprite;
sf::RectangleShape playerHealthBar[2], powerBar[2], reloadBar[2], minimapBackground, minimapViewFrame;
sf::CircleShape minimapPlayer, minimapEnemy;
sf::Music menuMusic, battleMusic, gameoverMusic, winMusic;
sf::SoundBuffer playerShootBuffer, enemyShootBuffer, getHealthBuffer, getPowerBuffer, getBombBuffer, getShieldBuffer,
					playerHitBuffer, enemyHitBuffer, playerDieBuffer, enemyDieBuffer, reloadBuffer, shieldBreakBuffer,
					UIbackBuffer, UIclickBuffer, UIswitchBuffer, UIchangeBuffer, stageClearBuffer, powerUpBuffer,
					beamShootBuffer, beamStartBuffer, bombHitBuffer;
sf::Sound playerShoot, enemyShoot, getHealth, getPower, getBomb, getShield,
			playerHit, enemyHit, playerDie, enemyDie, reload, shieldBreak,
			UIback, UIclick, UIswitch, UIchange, stageClear, powerUp,
			beamShoot, beamStart, bombHit;
bool win = 0;
bool lose = 0;
extern bool mainMenu, inGame, option, pause, specialThanks, quitWarning, saveWarning, pauseMenu, returnWarning;
int selectX = 0, selectY = 0, selectLimitX = 2, selectLimitY = 4;
int mainVolume = 100, SEvolume = 100, BGMvolume = 100;
std::ofstream osettings;
std::ifstream isettings;

double radian(double angle)
{
	return angle / 180 * pi;
}
double degree(double radian)
{
	return radian * 180 / pi;
}
double faceToward(sf::Vector2f origin, sf::Vector2f target)
{
	return (-degree(atan2f(target.x - origin.x, target.y - origin.y)));
}
int findNearestEnemy(sf::Vector2f coor, double direction, double angle)
{
	double min = 999999999;
	int minIndex = 0;
	for (int i = 0; i < 100; i++)
	{
		if (!enemies[i].getExist())
		{
			continue;
		}
		double distance = sqrt(pow(coor.x - enemies[i].getCoor().x, 2) + pow(coor.y - enemies[i].getCoor().y, 2));
		if (distance < min)
		{
			double enemyAngle = faceToward(coor, enemies[i].getCoor()) + 90;
			double deltaAngle = enemyAngle - direction;
			if (deltaAngle <= -180)
			{
				deltaAngle += 360;
			}
			else if (deltaAngle > 180)
			{
				deltaAngle -= 360;
			}
			if (deltaAngle <= angle / 2 && deltaAngle >= -angle / 2)
			{
				min = distance;
				minIndex = i;
			}
		}
	}
	if (min != 999999999)
	{
		return minIndex;
	}
	return -1;
}
int findNearestEnemy(sf::Vector2f coor)
{
	double min = 999999999;
	int minIndex = 0;
	for (int i = 0; i < 100; i++)
	{
		if (!enemies[i].getExist())
		{
			continue;
		}
		double distance = sqrt(pow(coor.x - enemies[i].getCoor().x, 2) + pow(coor.y - enemies[i].getCoor().y, 2));
		if (distance < min)
		{
			min = distance;
			minIndex = i;
		}
	}
	if (min != 999999999)
	{
		return minIndex;
	}
	return -1;
}
double tracking(sf::Vector2f origin, sf::Vector2f target, double direction, double turnSpeed)
{
	double angle = faceToward(origin, target) + 90;
	double deltaAngle = angle - direction;
	while (deltaAngle >= 270)
	{
		deltaAngle -= 360;
	}
	while (deltaAngle < -90)
	{
		deltaAngle += 360;
	}
	//		270/-90
	//  180		direction 0
	//			 90
	double finalAngle = 0;
	if (deltaAngle > 5 && deltaAngle <= 180)
	{
		finalAngle = direction + turnSpeed;
	}
	else if (deltaAngle <= 5 && deltaAngle >= -5)
	{
		finalAngle = angle;
	}
	else if ((deltaAngle < -5 && deltaAngle >= -90) || deltaAngle > 180 && deltaAngle <= 270)
	{
		finalAngle = direction - turnSpeed;
	}
	if (finalAngle >= 270)
	{
		return finalAngle - 360;
	}
	else if (finalAngle < -90)
	{
		return finalAngle + 360;
	}
	return finalAngle;
}
void ssClear()
{
	ss.clear();
	ss.str("");
}
bool isCollided(Character character, Bullet bullet)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (character.getHitbox(i).isCollided(bullet.getHitbox(j)))
			{
				return true;
			}
		}
	}
	return false;
}
bool isCollided(Character c1, Character c2)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (c1.getHitbox(i).isCollided(c2.getHitbox(j)))
			{
				return true;
			}
		}
	}
	return false;
}

int mapEditor()
{
	int map[30][54]
	{
	{6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,25,5,72,14,36,0,64,75,14,24,48,14,73,14,0,49,48,73,73,0,12,14,24},
	{6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,29,6,6,6,13,24,65,38,62,50,38,39,14,72,64,73,36,24,49,14,73,75,36,64,75,36},
	{6,6,1,2,2,2,2,3,6,6,6,6,6,6,6,6,6,34,6,6,6,6,6,6,6,6,6,6,6,6,6,6,25,26,26,26,5,76,48,61,62,38,50,62,38,62,50,62,62,38,38,50,62,62},
	{6,6,13,0,74,64,12,16,3,6,6,6,6,28,6,29,6,6,6,6,6,29,6,7,8,8,9,6,6,6,6,6,6,6,6,6,13,73,73,12,0,36,49,73,73,0,49,73,73,14,0,49,0,12},
	{6,6,13,74,48,41,4,5,16,3,6,6,6,6,6,6,6,6,6,6,6,6,6,19,42,79,22,8,9,6,6,6,28,6,6,6,25,26,26,26,5,75,76,73,73,48,51,73,73,36,65,63,48,24},
	{6,6,13,74,73,41,16,17,12,15,6,6,6,6,6,28,6,35,6,6,6,6,6,19,79,30,30,66,22,9,6,6,6,6,6,6,6,6,6,6,25,26,26,26,5,14,49,36,14,4,26,26,26,26},
	{6,6,13,24,73,50,36,24,74,15,6,6,6,6,6,6,6,6,1,2,3,6,6,31,32,32,32,32,32,33,6,29,6,6,6,6,6,6,6,6,6,6,6,6,13,65,53,77,24,15,6,6,6,6},
	{6,6,13,12,24,49,74,75,74,15,6,6,6,6,6,6,6,6,25,26,27,6,6,6,6,6,6,6,6,28,6,6,6,6,35,6,6,6,6,6,6,6,6,6,25,5,48,12,4,27,6,6,6,6},
	{6,6,13,75,72,40,36,12,0,15,6,6,6,6,6,6,6,6,6,6,6,6,6,6,35,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,25,26,26,27,6,6,6,7,8},
	{6,6,25,5,48,53,52,48,24,15,6,6,6,34,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,7,8,8,8,8,8,8,9,6,6,6,6,6,6,6,6,6,6,6,6,6,19,20},
	{6,6,6,13,74,72,49,74,12,16,3,6,6,6,6,6,28,6,29,6,6,6,6,6,6,6,6,6,7,8,8,23,20,20,18,20,20,30,21,6,6,6,6,6,6,6,6,6,6,6,6,6,19,18},
	{6,6,6,13,12,36,53,36,0,60,15,6,6,6,6,6,6,6,6,6,6,6,6,6,6,7,8,8,23,20,20,30,20,30,20,42,20,20,22,8,8,9,6,6,6,6,6,6,6,6,6,7,23,20},
	{6,6,6,13,74,24,74,24,12,75,15,6,6,29,6,7,9,6,6,6,6,6,6,6,6,19,20,20,20,20,20,54,20,81,20,20,20,20,66,20,20,22,8,9,6,6,6,6,6,6,6,19,20,20},
	{6,6,6,25,26,26,26,26,26,26,27,6,6,6,6,31,33,6,6,6,6,6,6,7,8,23,20,30,20,18,20,20,20,20,78,18,70,54,20,30,79,79,20,22,8,8,9,6,6,6,6,19,30,20},
	{6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,19,20,79,79,20,42,20,78,79,79,79,20,20,55,20,20,78,79,79,20,20,20,30,22,8,8,8,8,23,20,81},
	{6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,19,78,79,79,20,20,30,20,79,79,79,20,54,55,20,30,20,20,20,20,20,10,32,32,11,20,20,20,42,20,20},
	{6,6,6,6,6,7,8,8,8,8,8,8,8,8,9,6,6,6,6,6,6,6,6,31,11,20,66,78,20,20,20,79,79,79,43,44,59,44,45,20,18,20,81,20,21,6,35,31,11,18,20,20,20,20},
	{6,6,6,6,7,23,20,20,78,20,18,20,20,81,21,6,6,6,6,6,6,6,6,6,19,30,20,20,18,54,20,20,20,20,57,20,18,20,55,42,20,20,20,20,22,8,8,8,23,20,20,30,20,66},
	{6,6,7,8,23,20,20,30,20,20,20,20,70,20,21,6,6,6,6,6,6,6,6,6,19,20,71,44,68,44,45,20,30,20,55,54,20,20,67,44,45,20,30,20,20,20,20,20,20,20,20,20,20,20},
	{8,8,23,20,66,81,20,20,18,18,20,42,57,20,22,8,8,8,9,6,6,6,6,6,31,11,20,42,20,20,57,18,20,20,55,79,79,78,30,20,55,20,20,20,81,20,20,30,20,20,54,79,79,20},
	{20,20,20,20,20,20,79,79,54,78,79,79,55,30,20,66,20,20,21,6,6,6,6,6,6,19,20,20,20,30,57,20,42,20,57,79,79,20,20,20,55,42,20,20,20,20,20,20,20,20,20,79,79,20},
	{20,30,20,30,20,78,79,79,70,20,79,79,55,42,20,78,18,20,21,6,6,6,6,6,6,19,20,20,20,20,46,68,44,68,59,44,68,44,68,68,47,20,20,30,20,20,42,20,20,20,78,79,79,20},
	{20,20,20,81,30,18,20,20,55,30,20,20,55,20,30,20,20,81,21,6,6,6,6,6,6,31,11,20,81,20,55,30,20,20,20,18,54,79,79,79,57,20,78,79,79,20,20,20,20,81,20,20,20,20},
	{20,20,66,20,20,20,20,20,55,20,18,54,57,30,20,20,54,20,22,9,6,6,6,6,6,6,19,20,78,20,57,20,20,30,20,20,78,79,79,79,57,20,18,79,79,20,20,30,20,20,18,20,20,20},
	{30,20,20,79,79,42,43,44,56,44,44,44,69,79,79,79,20,20,20,21,6,6,6,6,6,6,19,20,20,20,55,20,20,20,20,20,20,20,20,20,55,54,20,20,20,20,54,18,20,20,20,20,30,20},
	{20,20,78,79,79,43,69,30,67,68,45,20,54,79,79,79,20,81,18,21,6,6,6,6,6,6,31,11,20,20,82,20,54,20,20,18,30,20,81,18,67,44,58,44,68,68,44,68,44,45,18,42,20,20},
	{20,30,20,54,20,55,30,20,79,79,55,20,78,79,79,79,20,30,20,22,9,6,6,6,6,6,6,31,11,20,20,78,79,79,79,20,20,20,20,20,20,18,55,20,42,20,20,20,20,67,68,44,44,68},
	{20,42,20,20,42,55,20,18,79,79,55,20,20,18,20,20,20,20,20,78,22,9,6,6,6,6,6,6,31,11,20,20,79,79,79,20,42,20,20,78,42,20,55,78,79,79,79,20,30,20,20,78,54,20},
	{20,44,68,44,68,69,20,81,42,78,55,30,20,20,20,20,78,20,66,20,20,22,9,6,6,6,6,6,6,31,11,20,79,79,81,20,20,30,20,20,20,20,57,20,79,79,79,20,20,20,66,30,20,20},
	{20,20,20,54,20,30,20,20,20,20,55,20,81,20,18,20,20,20,30,20,18,20,22,9,6,6,6,6,6,6,19,20,20,20,20,20,20,20,20,20,30,20,55,20,20,30,20,20,18,81,20,20,20,20}
	};
	sf::RenderWindow renderMap(sf::VideoMode(1920, 1080), "Map Editor", sf::Style::Default);

	sf::View view1(sf::FloatRect(0, 0, 1920 * 0.7, 1080));
	view1.setViewport(sf::FloatRect(0.f, 0.f, 0.7f, 1.f));
	sf::View view2(sf::FloatRect(-288, 0, 288, 672));
	view2.setViewport(sf::FloatRect(0.7f, 0.f, 0.3f, 1.0f));
	std::ofstream of;

	renderMap.setFramerateLimit(60);
	bool hold = 0;
	int type = 0, row = 0, column = 0;
	while (renderMap.isOpen())
	{
		sf::Event event;
		while (renderMap.pollEvent(event)) // event control
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				of.open("resources/Map output.txt");
				of << "int map[30][54]\n{\n";
				for (int i = 0; i < 30; i++)
				{
					of << '{';
					for (int j = 0; j < 54; j++)
					{
						of << map[i][j];
						if (j != 53)
						{
							of << ',';
						}
					}
					if (i == 29)
					{
						of << "}\n";
					}
					else
					{
						of << "},\n";
					}
				}
				of << "};" << endl;
				of.close();
				renderMap.close();
				break;
			case sf::Event::MouseWheelScrolled:
				if (event.mouseWheelScroll.delta > 0)
				{
					if (view2.getCenter().y > 336)
					{
						view2.move(0, -event.mouseWheelScroll.delta * 24);
					}
				}
				else
				{
					if (view2.getCenter().y < 336 + 48)
					{
						view2.move(0, -event.mouseWheelScroll.delta * 24);
					}
				}
				break;
			default:
				break;
			}
		}
		// input
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			view1.move(0, -10);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			view1.move(0, 10);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			view1.move(-10, 0);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			view1.move(10, 0);
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
		{
			renderMap.setView(view1);
			sf::Vector2i windowCoor = sf::Mouse::getPosition(renderMap);
			if (windowCoor.x > 0 && windowCoor.x < int(1920 * 0.7) && windowCoor.y > 0 && windowCoor.y < 1080)
			{
				sf::Vector2f coor = renderMap.mapPixelToCoords(windowCoor);
				int column = int(coor.x / 48);
				int row = int(coor.y / 48);
				map[row][column] = 6;
			}
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			renderMap.setView(view2);
			sf::Vector2f coor = renderMap.mapPixelToCoords(sf::Mouse::getPosition(renderMap));
			if (coor.x > -288 && coor.x < 0 && coor.y > 0 && coor.y < 672)
			{
				coor.x += 288.f;
				type = int(coor.y / 336);
				if (type == 1)
				{
					coor.y -= 336;
				}
				column = int(coor.x / 48);
				row = int(coor.y / 48);
				hold = true;
			}
			else
			{
				if (hold)
				{
					renderMap.setView(view1);
					sf::Vector2f coor = renderMap.mapPixelToCoords(sf::Mouse::getPosition(renderMap));
					cout << coor.x << "," << coor.y << endl;
					int targetColumn = int(coor.x / 48);
					int targetRow = int(coor.y / 48);
					map[targetRow][targetColumn] = type * 6 + row * 12 + column;
				}
			}
		}
		// draw
		renderMap.clear();
		sf::Sprite tileSprite;
		tileSprite.setTexture(tileTexture);
		renderMap.setView(view1);
		for (int i = 0; i < 30; i++)
		{
			for (int j = 0; j < 54; j++)
			{
				int column = map[i][j] % 12;
				int row = map[i][j] / 12;
				tileSprite.setTextureRect(sf::IntRect(column * 17, row * 17 + 51, 16, 16));
				tileSprite.setScale(3, 3);
				tileSprite.setPosition(j * 48.f, i * 48.f);
				renderMap.draw(tileSprite);
			}
		}
		renderMap.setView(view2);
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				for (int k = 0; k < 6; k++)
				{
					tileSprite.setTextureRect(sf::IntRect(k * 17 + i * 17 * 6, j * 17 + 51, 16, 16));
					tileSprite.setScale(3, 3);
					tileSprite.setPosition(k * 48.f - 288, j * 48.f + i * 336);
					renderMap.draw(tileSprite);
				}
			}
		}
		renderMap.setView(renderMap.getDefaultView());
		if (hold)
		{
			sf::Vector2f coor = sf::Vector2f(sf::Mouse::getPosition(renderMap));
			tileSprite.setTextureRect(sf::IntRect(column * 17 + type * 17 * 6, row * 17 + 51, 16, 16));
			tileSprite.setScale(3, 3);
			tileSprite.setOrigin(8, 8);
			tileSprite.setPosition(coor);
			renderMap.draw(tileSprite);
		}
		renderMap.display();
	}
	return 0;
}

void initialize()
{
	// variables
	int map[30][54]
	{
	{6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,25,5,72,14,36,0,64,75,14,24,48,14,73,14,0,49,48,73,73,0,12,14,24},
	{6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,29,6,6,6,13,24,65,38,62,50,38,39,14,72,64,73,36,24,49,14,73,75,36,64,75,36},
	{6,6,1,2,2,2,2,3,6,6,6,6,6,6,6,6,6,34,6,6,6,6,6,6,6,6,6,6,6,6,6,6,25,26,26,26,5,76,48,61,62,38,50,62,38,62,50,62,62,38,38,50,62,62},
	{6,6,13,0,74,64,12,16,3,6,6,6,6,28,6,29,6,6,6,6,6,29,6,7,8,8,9,6,6,6,6,6,6,6,6,6,13,73,73,12,0,36,49,73,73,0,49,73,73,14,0,49,0,12},
	{6,6,13,74,48,41,4,5,16,3,6,6,6,6,6,6,6,6,6,6,6,6,6,19,42,79,22,8,9,6,6,6,28,6,6,6,25,26,26,26,5,75,76,73,73,48,51,73,73,36,65,63,48,24},
	{6,6,13,74,73,41,16,17,12,15,6,6,6,6,6,28,6,35,6,6,6,6,6,19,79,30,30,66,22,9,6,6,6,6,6,6,6,6,6,6,25,26,26,26,5,14,49,36,14,4,26,26,26,26},
	{6,6,13,24,73,50,36,24,74,15,6,6,6,6,6,6,6,6,1,2,3,6,6,31,32,32,32,32,32,33,6,29,6,6,6,6,6,6,6,6,6,6,6,6,13,65,53,77,24,15,6,6,6,6},
	{6,6,13,12,24,49,74,75,74,15,6,6,6,6,6,6,6,6,25,26,27,6,6,6,6,6,6,6,6,28,6,6,6,6,35,6,6,6,6,6,6,6,6,6,25,5,48,12,4,27,6,6,6,6},
	{6,6,13,75,72,40,36,12,0,15,6,6,6,6,6,6,6,6,6,6,6,6,6,6,35,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,25,26,26,27,6,6,6,7,8},
	{6,6,25,5,48,53,52,48,24,15,6,6,6,34,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,7,8,8,8,8,8,8,9,6,6,6,6,6,6,6,6,6,6,6,6,6,19,20},
	{6,6,6,13,74,72,49,74,12,16,3,6,6,6,6,6,28,6,29,6,6,6,6,6,6,6,6,6,7,8,8,23,20,20,18,20,20,30,21,6,6,6,6,6,6,6,6,6,6,6,6,6,19,18},
	{6,6,6,13,12,36,53,36,0,60,15,6,6,6,6,6,6,6,6,6,6,6,6,6,6,7,8,8,23,20,20,30,20,30,20,42,20,20,22,8,8,9,6,6,6,6,6,6,6,6,6,7,23,20},
	{6,6,6,13,74,24,74,24,12,75,15,6,6,29,6,7,9,6,6,6,6,6,6,6,6,19,20,20,20,20,20,54,20,81,20,20,20,20,66,20,20,22,8,9,6,6,6,6,6,6,6,19,20,20},
	{6,6,6,25,26,26,26,26,26,26,27,6,6,6,6,31,33,6,6,6,6,6,6,7,8,23,20,30,20,18,20,20,20,20,78,18,70,54,20,30,79,79,20,22,8,8,9,6,6,6,6,19,30,20},
	{6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,19,20,79,79,20,42,20,78,79,79,79,20,20,55,20,20,78,79,79,20,20,20,30,22,8,8,8,8,23,20,81},
	{6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,19,78,79,79,20,20,30,20,79,79,79,20,54,55,20,30,20,20,20,20,20,10,32,32,11,20,20,20,42,20,20},
	{6,6,6,6,6,7,8,8,8,8,8,8,8,8,9,6,6,6,6,6,6,6,6,31,11,20,66,78,20,20,20,79,79,79,43,44,59,44,45,20,18,20,81,20,21,6,35,31,11,18,20,20,20,20},
	{6,6,6,6,7,23,20,20,78,20,18,20,20,81,21,6,6,6,6,6,6,6,6,6,19,30,20,20,18,54,20,20,20,20,57,20,18,20,55,42,20,20,20,20,22,8,8,8,23,20,20,30,20,66},
	{6,6,7,8,23,20,20,30,20,20,20,20,70,20,21,6,6,6,6,6,6,6,6,6,19,20,71,44,68,44,45,20,30,20,55,54,20,20,67,44,45,20,30,20,20,20,20,20,20,20,20,20,20,20},
	{8,8,23,20,66,81,20,20,18,18,20,42,57,20,22,8,8,8,9,6,6,6,6,6,31,11,20,42,20,20,57,18,20,20,55,79,79,78,30,20,55,20,20,20,81,20,20,30,20,20,54,79,79,20},
	{20,20,20,20,20,20,79,79,54,78,79,79,55,30,20,66,20,20,21,6,6,6,6,6,6,19,20,20,20,30,57,20,42,20,57,79,79,20,20,20,55,42,20,20,20,20,20,20,20,20,20,79,79,20},
	{20,30,20,30,20,78,79,79,70,20,79,79,55,42,20,78,18,20,21,6,6,6,6,6,6,19,20,20,20,20,46,68,44,68,59,44,68,44,68,68,47,20,20,30,20,20,42,20,20,20,78,79,79,20},
	{20,20,20,81,30,18,20,20,55,30,20,20,55,20,30,20,20,81,21,6,6,6,6,6,6,31,11,20,81,20,55,30,20,20,20,18,54,79,79,79,57,20,78,79,79,20,20,20,20,81,20,20,20,20},
	{20,20,66,20,20,20,20,20,55,20,18,54,57,30,20,20,54,20,22,9,6,6,6,6,6,6,19,20,78,20,57,20,20,30,20,20,78,79,79,79,57,20,18,79,79,20,20,30,20,20,18,20,20,20},
	{30,20,20,79,79,42,43,44,56,44,44,44,69,79,79,79,20,20,20,21,6,6,6,6,6,6,19,20,20,20,55,20,20,20,20,20,20,20,20,20,55,54,20,20,20,20,54,18,20,20,20,20,30,20},
	{20,20,78,79,79,43,69,30,67,68,45,20,54,79,79,79,20,81,18,21,6,6,6,6,6,6,31,11,20,20,82,20,54,20,20,18,30,20,81,18,67,44,58,44,68,68,44,68,44,45,18,42,20,20},
	{20,30,20,54,20,55,30,20,79,79,55,20,78,79,79,79,20,30,20,22,9,6,6,6,6,6,6,31,11,20,20,78,79,79,79,20,20,20,20,20,20,18,55,20,42,20,20,20,20,67,68,44,44,68},
	{20,42,20,20,42,55,20,18,79,79,55,20,20,18,20,20,20,20,20,78,22,9,6,6,6,6,6,6,31,11,20,20,79,79,79,20,42,20,20,78,42,20,55,78,79,79,79,20,30,20,20,78,54,20},
	{20,44,68,44,68,69,20,81,42,78,55,30,20,20,20,20,78,20,66,20,20,22,9,6,6,6,6,6,6,31,11,20,79,79,81,20,20,30,20,20,20,20,57,20,79,79,79,20,20,20,66,30,20,20},
	{20,20,20,54,20,30,20,20,20,20,55,20,81,20,18,20,20,20,30,20,18,20,22,9,6,6,6,6,6,6,19,20,20,20,20,20,20,20,20,20,30,20,55,20,20,30,20,20,18,81,20,20,20,20}
	};
	int ui[3][5]
	{
		{620, 621, 621, 621, 622},
		{643, 644, 644, 644, 645},
		{650, 651, 651, 651, 652}
	};
	int scorePanel[3][6]
	{
		{264, 265, 265, 265, 265, 266},
		{294, 295, 295, 295, 295, 296},
		{324, 325, 325, 325, 325, 326}
	};
	sf::Texture tempTexture;
	sf::Sprite tempSprite;
	//music
	menuMusic.setLoop(true);
	if (!menuMusic.openFromFile("resources/musics/menuMusic.ogg"))
	{
		cout << "failed to load menuMusic.ogg" << endl;
	}
	battleMusic.setLoop(true);
	if (!battleMusic.openFromFile("resources/musics/battleMusic.ogg"))
	{
		cout << "failed to load battleMusic.ogg" << endl;
	}
	gameoverMusic.setLoop(true);
	if (!gameoverMusic.openFromFile("resources/musics/gameoverMusic.ogg"))
	{
		cout << "failed to load gameoverMusic.ogg" << endl;
	}
	winMusic.setLoop(true);
	if (!winMusic.openFromFile("resources/musics/winMusic.ogg"))
	{
		cout << "failed to load winMusic.ogg" << endl;
	}
	// sound buffer
	if (!playerShootBuffer.loadFromFile("resources/sounds/playerShoot.ogg"))
	{
		cout << "failed to load playerShoot.ogg" << endl;;
	}
	if (!enemyShootBuffer.loadFromFile("resources/sounds/enemyShoot.ogg"))
	{
		cout << "failed to load enemyShoot.ogg" << endl;
	}
	if (!getHealthBuffer.loadFromFile("resources/sounds/getHealth.wav"))
	{
		cout << "failed to load getHealth.wav" << endl;
	}
	if (!getPowerBuffer.loadFromFile("resources/sounds/getPower.wav"))
	{
		cout << "failed to load getPower.wav" << endl;
	}
	if (!getBombBuffer.loadFromFile("resources/sounds/getBomb.ogg"))
	{
		cout << "failed to load getBomb.ogg" << endl;
	}
	if (!getShieldBuffer.loadFromFile("resources/sounds/getShield.ogg"))
	{
		cout << "failed to load getShield.ogg" << endl;
	}
	if (!playerHitBuffer.loadFromFile("resources/sounds/playerHit.wav"))
	{
		cout << "failed to load playerHit.wav" << endl;
	}
	if (!enemyHitBuffer.loadFromFile("resources/sounds/enemyHit.wav"))
	{
		cout << "failed to load enemyHit.wav" << endl;
	}
	if (!playerDieBuffer.loadFromFile("resources/sounds/playerDie.wav"))
	{
		cout << "failed to load playerDie.wav" << endl;
	}
	if (!enemyDieBuffer.loadFromFile("resources/sounds/enemyDie.wav"))
	{
		cout << "failed to load enemyDie.wav" << endl;
	}
	if (!reloadBuffer.loadFromFile("resources/sounds/reload.ogg"))
	{
		cout << "failed to load reload.ogg" << endl;
	}
	if (!shieldBreakBuffer.loadFromFile("resources/sounds/shieldBreak.wav"))
	{
		cout << "failed to load shieldBreak.wav" << endl;
	}
	if (!UIbackBuffer.loadFromFile("resources/sounds/UIback.ogg"))
	{
		cout << "failed to load UIback.ogg" << endl;
	}
	if (!UIclickBuffer.loadFromFile("resources/sounds/UIclick.ogg"))
	{
		cout << "failed to load UIclick.ogg" << endl;
	}
	if (!UIswitchBuffer.loadFromFile("resources/sounds/UIswitch.ogg"))
	{
		cout << "failed to load UIswitch.ogg" << endl;
	}
	if (!UIchangeBuffer.loadFromFile("resources/sounds/UIchange.wav"))
	{
		cout << "failed to load UIchange.wav" << endl;
	}
	if (!stageClearBuffer.loadFromFile("resources/sounds/stageClear.wav"))
	{
		cout << "failed to load stageClear.wav" << endl;
	}
	if (!powerUpBuffer.loadFromFile("resources/sounds/powerUp.wav"))
	{
		cout << "failed to load powerUp.wav" << endl;
	}
	if (!beamShootBuffer.loadFromFile("resources/sounds/beamShoot.ogg"))
	{
		cout << "failed to load beamShoot.ogg" << endl;
	}
	if (!beamStartBuffer.loadFromFile("resources/sounds/beamStart.ogg"))
	{
		cout << "failed to load beamStart.ogg" << endl;
	}
	if (!bombHitBuffer.loadFromFile("resources/sounds/bombHit.ogg"))
	{
		cout << "failed to load bombHit.ogg" << endl;
	}
	//sound
	playerShoot.setBuffer(playerShootBuffer);
	enemyShoot.setBuffer(enemyShootBuffer);
	getHealth.setBuffer(getHealthBuffer);
	getPower.setBuffer(getPowerBuffer);
	getBomb.setBuffer(getBombBuffer);
	getShield.setBuffer(getShieldBuffer);
	playerHit.setBuffer(playerHitBuffer);
	enemyHit.setBuffer(enemyHitBuffer);
	playerDie.setBuffer(playerDieBuffer);
	enemyDie.setBuffer(enemyDieBuffer);
	reload.setBuffer(reloadBuffer);
	shieldBreak.setBuffer(shieldBreakBuffer);
	UIback.setBuffer(UIbackBuffer);
	UIclick.setBuffer(UIclickBuffer);
	UIswitch.setBuffer(UIswitchBuffer);
	UIchange.setBuffer(UIchangeBuffer);
	stageClear.setBuffer(stageClearBuffer);
	powerUp.setBuffer(powerUpBuffer);
	beamShoot.setBuffer(beamShootBuffer);
	beamStart.setBuffer(beamStartBuffer);
	bombHit.setBuffer(bombHitBuffer);
	// volume
	isettings.open("resources/settings.txt");
	std::string str;
	isettings >> str >> str;
	ss << str;
	ss >> mainVolume;
	ssClear();
	isettings >> str >> str;
	ss << str;
	ss >> SEvolume;
	ssClear();
	isettings >> str >> str;
	ss << str;
	ss >> BGMvolume;
	ssClear();
	isettings.close();
	updateVolume();
	menuMusic.play();
	// others
	window.setFramerateLimit(60);
	srand((unsigned)time(NULL));
	// view
	window.setView(view);
	minimap.setViewport(sf::FloatRect(0.8f, 0.f, 0.2f, 0.2f));
	minimapBackground.setFillColor(sf::Color::Black);
	minimapBackground.setSize(sf::Vector2f(mapSize.x + 20.f, mapSize.y + 20.f));
	minimapBackground.setPosition(sf::Vector2f(-10, -10));
	minimapPlayer.setFillColor(sf::Color::Blue);
	minimapPlayer.setRadius(60);
	minimapPlayer.setOrigin(minimapPlayer.getGlobalBounds().width / 2, minimapPlayer.getGlobalBounds().height / 2);
	minimapEnemy.setFillColor(sf::Color::Red);
	minimapEnemy.setRadius(30);
	minimapEnemy.setOrigin(minimapEnemy.getGlobalBounds().width / 2, minimapEnemy.getGlobalBounds().height / 2);
	minimapViewFrame.setFillColor(sf::Color::Transparent);
	minimapViewFrame.setOutlineColor(sf::Color(255, 255, 255, 200));
	minimapViewFrame.setSize(sf::Vector2f(1280, 720));
	minimapViewFrame.setOutlineThickness(10);
	minimapViewFrame.setOrigin(1280 / 2, 720 / 2);
	// mouse
	window.setMouseCursorGrabbed(false);
	window.setMouseCursorVisible(false);
	mouseSprite.setTexture(tileTexture);
	mouseSprite.setTextureRect(sf::IntRect(51, 34, 16, 16));
	mouseSprite.setScale(sf::Vector2f(2.5, 2.5));
	mouseSprite.setOrigin(mouseSprite.getLocalBounds().width / 2, mouseSprite.getLocalBounds().height / 2);
	// font & text
	fontEn.loadFromFile("resources/fonts/Kenney Pixel.ttf");
	fontCh.loadFromFile("resources/fonts/Pixel.ttf");
	debugText.setFont(fontEn);	
	debugText.setCharacterSize(45);
	debugText.setFillColor(sf::Color::Black);
	debugText.setOutlineColor(sf::Color::White);
	debugText.setOutlineThickness(1);
	scoreText.setFillColor(sf::Color::Black);
	scoreText.setCharacterSize(45);
	scoreText.setFont(fontEn);
	scoreText.setString("Score 000000000");
	timeText.setFillColor(sf::Color::Black);
	timeText.setCharacterSize(45);
	timeText.setFont(fontEn);
	timeText.setString("Time 00:00");
	stageText.setFillColor(sf::Color::Black);
	stageText.setCharacterSize(45);
	stageText.setFont(fontEn);
	stageText.setString("Stage 1/10");
	// textures
	if (!shipTexture.loadFromFile("resources/images/ships.png"))
	{
		cout << "Failed to open ships.png" << endl;
	}
	if (!tileTexture.loadFromFile("resources/images/tiles.png"))
	{
		cout << "Failed to open tiles.png" << endl;
	}
	if (!characterTexture.loadFromFile("resources/images/characters.png"))
	{
		cout << "Failed to open character.png" << endl;
	}
	if (!bulletHitTexture.loadFromFile("resources/images/bulletHit.png"))
	{
		cout << "Failed to open bulletHit.png" << endl;
	}
	if (!enemyDieTexture.loadFromFile("resources/images/enemyDie.png"))
	{
		cout << "Failed to open enemyDie.png" << endl;
	}
	if (!uiTexture.loadFromFile("resources/images/UIpackSheet_transparent.png"))
	{
		cout << "Failed to load UIpackSheet_transparent.png" << endl;
	}
	if (!iconTexture.loadFromFile("resources/images/icon.png"))
	{
		cout << "Failed to load icon.png" << endl;
	}
	if (!itemTexture.loadFromFile("resources/images/item.png"))
	{
		cout << "Failed to load item.png" << endl;
	}
	if (!bulletTexture.loadFromFile("resources/images/bullet.png"))
	{
		cout << "Failed to load bullet.png" << endl;
	}
	if (!selectCursorTexture.loadFromFile("resources/images/selectCursor.png"))
	{
		cout << "failed to load selectCursor.png" << endl;
	}
	if (!SFMLTexture.loadFromFile("resources/images/SFML.png"))
	{
		cout << "failed to load SFML.png" << endl;
	}
	if (!MODTexture.loadFromFile("resources/images/maoudamashi.png"))
	{
		cout << "failed to load maoudamashi.png" << endl;
	}
	if (!kenneyTexture.loadFromFile("resources/images/kenney.png"))
	{
		cout << "failed to load kenney.png" << endl;
	}
	// map
	mapTexture.create(mapSize.x, mapSize.y);
	sf::Sprite tileSprite;
	tileSprite.setTexture(tileTexture);
	for (int i = 0; i < mapSize.y / 48; i++)
	{
		for (int j = 0; j < mapSize.x / 48 + 1; j++)
		{
			int column = map[i][j] % 12;
			int row = map[i][j] / 12;
			tileSprite.setTextureRect(sf::IntRect(column * 17, row * 17 + 51, 16, 16));
			tileSprite.setScale(3, 3);
			tileSprite.setPosition(j * 48.f, i * 48.f);
			mapTexture.draw(tileSprite);
		}
	}
	mapTexture.display();
	mapSprite.setTexture(mapTexture.getTexture());
	// UI
	uiRenderTexture.create(uiSize.x, uiSize.y);
	tileSprite.setTexture(uiTexture);
	for (int i = 0; i < uiSize.y / 48; i++)
	{
		for (int j = 0; j < uiSize.x / 48; j++)
		{
			int row = int(ui[i][j] / 30);
			int column = ui[i][j] % 30;
			tileSprite.setTextureRect(sf::IntRect(column * 18, row * 18, 16, 16));
			tileSprite.setScale(3, 3);
			tileSprite.setPosition(j * 48.f, i * 48.f);
			uiRenderTexture.draw(tileSprite);
		}
	}
	tileSprite.setTexture(iconTexture);
	for (int i = 0; i < 4; i++)
	{
		tileSprite.setTextureRect(sf::IntRect(i * 16, 0, 16, 16));
		tileSprite.setScale(1.5, 1.5);
		tileSprite.setPosition(16, 16 + i * 30.f);
		uiRenderTexture.draw(tileSprite);
	}
	uiRenderTexture.display();
	uiSprite.setTexture(uiRenderTexture.getTexture());
	playerHealthBar[1].setFillColor(sf::Color::Red);
	playerHealthBar[0].setFillColor(sf::Color(128, 128, 128, 255));
	powerBar[1].setFillColor(sf::Color(230, 230, 0, 255));
	powerBar[0].setFillColor(sf::Color(128, 128, 128, 255));
	reloadBar[1].setFillColor(sf::Color(220, 220, 0, 255));
	reloadBar[0].setFillColor(sf::Color(128, 128, 128, 255));
	for (int i = 0; i < 2; i++)
	{
		ammoSprite[i].setTexture(iconTexture);
		ammoSprite[i].setTextureRect(sf::IntRect(16 - 16 * i, 16, 16, 16));
		ammoSprite[i].setScale(1.5, 1.5);
		bombAmmoSprite[i].setTexture(iconTexture);
		bombAmmoSprite[i].setTextureRect(sf::IntRect(48 - 16 * i, 16, 16, 16));
		bombAmmoSprite[i].setScale(1.5, 1.5);
	}
	// score panel
	scorePanelTexture.create(48 * 6, 48 * 3);
	tileSprite.setTexture(uiTexture);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			int row = int(scorePanel[i][j] / 30);
			int column = scorePanel[i][j] % 30;
			tileSprite.setTextureRect(sf::IntRect(column * 18, row * 18, 16, 16));
			tileSprite.setScale(3, 3);
			tileSprite.setPosition(j * 48.f, i * 48.f);
			scorePanelTexture.draw(tileSprite);
		}
	}
	scorePanelTexture.display();
	scorePanelSprite.setTexture(scorePanelTexture.getTexture());
	scorePanelSprite.setColor(sf::Color(255, 255, 255, 200));
	// ship sprite
	for (auto& x : shipSprite)
	{
		x.setTexture(shipTexture);
	}
	shipSprite[0].setTextureRect(sf::IntRect(33, 66, 32, 32));
	shipSprite[1].setTextureRect(sf::IntRect(33, 33, 32, 32));
	shipSprite[2].setTextureRect(sf::IntRect(33, 0, 32, 32));
	for (auto& x : shipSprite)
	{
		x.setScale(3, 3);
		x.setOrigin(x.getLocalBounds().width / 2, x.getLocalBounds().height / 2);
	}
	//bullet sprite
	for (int i = 0; i < 4; i++)
	{
		bulletSprite[i].setTexture(tileTexture);
	}
	for (int i = 4; i < 25; i++)
	{
		bulletSprite[i].setTexture(bulletTexture);
	}
	bulletSprite[0].setTextureRect(sf::IntRect(34, 0, 16, 16));
	bulletSprite[1].setTextureRect(sf::IntRect(0, 0, 16, 16));
	bulletSprite[2].setTextureRect(sf::IntRect(17, 0, 16, 16));
	bulletSprite[3].setTextureRect(sf::IntRect(51, 0, 16, 16));
	for (int i = 0; i < 4; i++)
	{
		bulletSprite[i].setScale(3, 3);
	}
	for (int i = 4; i < 11; i++)
	{
		bulletSprite[i].setTextureRect(sf::IntRect((i - 4) * 20, 0, 20, 20));
		bulletSprite[i].setScale(2, 2);
	}
	for (int i = 11; i < 18; i++)
	{
		bulletSprite[i].setTextureRect(sf::IntRect((i - 11) * 10, 20, 10, 15));
		bulletSprite[i].setScale(2, 2);
	}
	for (int i = 18; i < 25; i++)
	{
		bulletSprite[i].setTextureRect(sf::IntRect((i - 18) * 10, 35, 10, 12));
		bulletSprite[i].setScale(2, 2);
	}
	for (auto& x : bulletSprite)
	{
		x.setOrigin(x.getLocalBounds().width / 2, x.getLocalBounds().height / 2);
	}
	// bomb sprite
	for (auto& x : bombSprite)
	{
		x.setTexture(tileTexture);
	}
	bombSprite[0].setTextureRect(sf::IntRect(0, 17, 16, 16));
	bombSprite[1].setTextureRect(sf::IntRect(17, 17, 16, 16));
	bombSprite[2].setTextureRect(sf::IntRect(51, 17, 16, 16));
	bombSprite[3].setTextureRect(sf::IntRect(51, 0, 16, 16));
	for (auto& x : bombSprite)
	{
		x.setScale(4.5, 4.5);
		x.setOrigin(x.getLocalBounds().width / 2, x.getLocalBounds().height / 2);
	}
	// enemy sprite
	for (auto& x : enemySprite)
	{
		x.setTexture(characterTexture);
	}
	for (int i = 0; i < 8; i++)
	{
		enemySprite[i].setTextureRect(sf::IntRect(i * 26, 0, 24, 24));
	}
	for (int i = 8; i < 14; i++)
	{
		enemySprite[i].setTextureRect(sf::IntRect((i - 8) * 26, 26, 24, 24));
	}
	enemySprite[14].setTextureRect(sf::IntRect(208, 0, 24, 24));
	enemySprite[15].setTextureRect(sf::IntRect(156, 52, 24, 24));
	enemySprite[16].setTextureRect(sf::IntRect(182, 52, 24, 24));
	enemySprite[17].setTextureRect(sf::IntRect(208, 52, 24, 24));
	for (auto& x : enemySprite)
	{
		x.setScale(3, 3);
		x.setOrigin(x.getLocalBounds().width / 2, x.getLocalBounds().height / 2);
	}
	// item sprite
	for (int i = 0; i < 4; i++)
	{
		itemSprite[i].setTexture(itemTexture);
		itemSprite[i].setTextureRect(sf::IntRect(i * 16, 0, 16, 16));
		itemSprite[i].setScale(2, 2);
		itemSprite[i].setOrigin(itemSprite[i].getLocalBounds().width / 2, itemSprite[i].getLocalBounds().height / 2);
	}
	// shield sprite
	shieldTexture.create(120, 120);
	sf::VertexArray varr(sf::TriangleFan, 38);
	varr[0].color = sf::Color(255, 255, 255, 64);
	varr[0].position = sf::Vector2f(60, 60);
	for (int i = 1; i < 38; i++)
	{
		varr[i].color = sf::Color(128, 200, 255, 255);
		varr[i].position = varr[0].position + sf::Vector2f(float(cos(radian((i - 1) * 10)) * 60), float(sin(radian((i - 1) * 10)) * 60));
	}
	shieldTexture.draw(varr);
	sf::CircleShape circle;
	circle.setRadius(55);
	circle.setOrigin(circle.getGlobalBounds().width / 2, circle.getGlobalBounds().height / 2);
	circle.setPosition(varr[0].position);
	circle.setFillColor(sf::Color::Transparent);
	circle.setOutlineColor(sf::Color(255, 255, 255, 200));
	circle.setOutlineThickness(3);
	shieldTexture.draw(circle);
	shieldTexture.display();
	shieldSprite.setTexture(shieldTexture.getTexture());
	shieldSprite.setOrigin(shieldSprite.getGlobalBounds().width / 2, shieldSprite.getGlobalBounds().height / 2);
	// animation sprite
	bulletHitSprite.setTexture(bulletHitTexture);
	bulletHitSprite.setScale(3, 3);
	bulletHitSprite.setTextureRect(sf::IntRect(0, 0, 16, 16));
	bulletHitSprite.setOrigin(8, 8);
	bombHitSprite.setTexture(bulletHitTexture);
	bombHitSprite.setScale(10, 10);
	bombHitSprite.setTextureRect(sf::IntRect(0, 0, 16, 16));
	bombHitSprite.setOrigin(8, 8);
	enemyDieSprite.setTexture(enemyDieTexture);
	enemyDieSprite.setScale(3, 3);
	enemyDieSprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
	enemyDieSprite.setOrigin(16, 16);
	shieldBreakSprite.setTexture(shieldTexture.getTexture());
	shieldBreakSprite.setOrigin(shieldBreakSprite.getGlobalBounds().width / 2, shieldBreakSprite.getGlobalBounds().height / 2);
	// beam sprite
	beamTexture.create(1440, 48);
	tempSprite.setTexture(bulletTexture);
	tempSprite.setScale(3, 3);
	tempSprite.setTextureRect(sf::IntRect(0, 47, 16, 16));
	tempSprite.setPosition(0, 0);
	beamTexture.draw(tempSprite);
	tempSprite.setTextureRect(sf::IntRect(16, 47, 16, 16));
	for (int i = 0; i < 28; i++)
	{
		tempSprite.setPosition(48 + i * 48, 0);
		beamTexture.draw(tempSprite);
	}
	tempSprite.setTextureRect(sf::IntRect(32, 47, 16, 16));
	tempSprite.setPosition(1440 - 48, 0);
	beamTexture.draw(tempSprite);
	beamTexture.display();
	beamSprite.setTexture(beamTexture.getTexture());
	beamSprite.setOrigin(0, beamSprite.getGlobalBounds().height / 2);
	// notice frame sprite
	noticeFrameTexture.create(600, 50);
	sf::RectangleShape tempRect(sf::Vector2f(596, 46));
	tempRect.setFillColor(sf::Color::Black);
	tempRect.setOutlineThickness(2);
	tempRect.setOutlineColor(sf::Color::White);
	tempRect.setPosition(2, 2);
	noticeFrameTexture.draw(tempRect);
	tempRect.setSize(sf::Vector2f(588, 38));
	tempRect.setPosition(6, 6);
	noticeFrameTexture.draw(tempRect);
	noticeFrameTexture.display();
	noticeFrameSprite.setTexture(noticeFrameTexture.getTexture());
	// select cursor sprite
	selectCursorSprite.setTexture(selectCursorTexture);
	selectCursorSprite.setScale(3, 3);
	// warning frame sprite
	warningFrameTexture.create(640, 360);
	tempRect.setSize(sf::Vector2f(636, 356));
	tempRect.setPosition(2, 2);
	warningFrameTexture.draw(tempRect);
	tempRect.setSize(sf::Vector2f(628, 348));
	tempRect.setPosition(6, 6);
	warningFrameTexture.draw(tempRect);
	warningFrameTexture.display();
	warningFrameSprite.setTexture(warningFrameTexture.getTexture());
	warningFrameSprite.setOrigin(warningFrameSprite.getGlobalBounds().width / 2, warningFrameSprite.getGlobalBounds().height / 2);
	warningFrameSprite.setPosition(1280 / 2, 720 / 2);
	//special thanks sprite
	SFMLSprite.setTexture(SFMLTexture);
	SFMLSprite.setPosition(350, 100);
	SFMLSprite.setScale(0.3, 0.3);
	MODSprite.setTexture(MODTexture);
	MODSprite.setPosition(300, 270);
	MODSprite.setScale(0.5, 0.5);
	kenneySprite.setTexture(kenneyTexture);
}
void gameInitialize()
{
	// stage
	stage = Stage(1);
	// player
	playerShip.setHealth(100);
	playerShip.setPower(20);
	playerShip.setShield(true);
	playerShip.setBomb(3);
	playerShip.updateLevel();
	playerShip.setAmmo(playerShip.getMaxAmmo());
	playerShip.setCoor(mapSize.x / 2, mapSize.y / 2);
	playerShip.setExist(true);
	// other
	score = 0;
	frametime = 0;
	timeSec = 0;
	// objects
	for (auto& x : enemies)
	{
		x.setExist(false);
	}
	for (auto& x : items)
	{
		x.setExist(false);
	}
	for (auto& x : bullets)
	{
		x.setExist(false);
	}
	for (auto& x : bombs)
	{
		x.setExist(false);
	}
	for (auto& x : beams)
	{
		x.setExist(false);
	}
	for (auto& x : animations)
	{
		x.setExist(false);
	}
	for (auto& x : notices)
	{
		x.setExist(false);
	}
	// music
	battleMusic.play();
}

void update()
{
	if (inGame && !pause)
	{
		//stage
		stage.update();
		//time
		frametime++;
		if (frametime % 60 == 0)
		{
			timeSec++;
		}
		//view
		view.setCenter(playerShip.getCoor());
		if (view.getCenter().x - view.getSize().x / 2 < 0)
		{
			view.setCenter(view.getSize().x / 2, view.getCenter().y);
		}
		else if (view.getCenter().x + view.getSize().x / 2 > mapSize.x)
		{
			view.setCenter(mapSize.x - view.getSize().x / 2, view.getCenter().y);
		}
		if (view.getCenter().y - view.getSize().y / 2 < 0)
		{
			view.setCenter(view.getCenter().x, view.getSize().y / 2);
		}
		else if (view.getCenter().y + view.getSize().y / 2 > mapSize.y)
		{
			view.setCenter(view.getCenter().x, mapSize.y - view.getSize().y / 2);
		}
		window.setView(view);
		//mouse
		sf::Vector2f coor = window.mapPixelToCoords(sf::Mouse::getPosition(window));
		mouseSprite.setPosition(coor);
		//debug text
		{
			std::string text;
			ss << playerShip.getAcceleration();
			text += "Acceleration: " + ss.str() + '\n';
			ssClear();
			ss << playerShip.getAngle();
			text += "Angle: " + ss.str() + '\n';
			ssClear();
			ss << playerShip.getBrake();
			text += "Brake: " + ss.str() + '\n';
			ssClear();
			ss << playerShip.getX();
			text += "Coordinate: (" + ss.str();
			ssClear();
			ss << playerShip.getY();
			text += " , " + ss.str() + ")\n";
			ssClear();
			ss << stage.getSpawnLimit();
			text += "Spawn Limit : " + ss.str() + "\n";
			ssClear();
			ss << stage.getSpawnCount();
			text += "Spawn Count : " + ss.str() + "\n";
			ssClear();
			ss << stage.getKilledEnemy();
			text += "Killed Enemy : " + ss.str() + "\n";
			ssClear();
			debugText.setPosition(sf::Vector2f(window.mapPixelToCoords(sf::Vector2i(0, 0), view)));
			debugText.setString(text);
		}
		//player
		playerShip.update();
		//bullets
		for (auto& x : bullets)
		{
			x.update();
		}
		//bombs
		for (auto& x : bombs)
		{
			x.update();
		}
		//beams
		for (auto& x : beams)
		{
			x.update();
		}
		//enemy
		for (auto& x : enemies)
		{
			x.update();
		}
		// item
		for (auto& x : items)
		{
			x.update();
		}
		//animation
		for (auto& x : animations)
		{
			x.update();
		}
		//shield
		if (playerShip.getShield())
		{
			shieldSprite.setPosition(playerShip.getCoor());
		}
		//notice
		for (auto& x : notices)
		{
			x.update();
		}
		//UI
		uiSprite.setPosition(view.getCenter() + sf::Vector2f(-1280 / 2, 720 / 2 - 48 * 3));
		double percent = playerShip.getHealth() / playerShip.getMaxHealth();
		playerHealthBar[1].setPosition(uiSprite.getPosition() + sf::Vector2f(45, 19));
		playerHealthBar[1].setSize(sf::Vector2f(180 * float(percent), 18));
		playerHealthBar[0].setPosition(playerHealthBar[1].getPosition() + sf::Vector2f(180 * float(percent), 0));
		playerHealthBar[0].setSize(sf::Vector2f(180 * float(1 - percent), 18));
		percent = playerShip.getPower() / playerShip.getMaxPower();
		powerBar[1].setPosition(uiSprite.getPosition() + sf::Vector2f(45, 49));
		powerBar[1].setSize(sf::Vector2f(180 * float(percent), 18));
		powerBar[0].setPosition(powerBar[1].getPosition() + sf::Vector2f(180 * float(percent), 0));
		powerBar[0].setSize(sf::Vector2f(180 * float(1 - percent), 18));
		// score panel
		scorePanelSprite.setPosition(view.getCenter() + sf::Vector2f(-640, -360));
		scoreText.setPosition(view.getCenter() + sf::Vector2f(-620, -370));
		timeText.setPosition(view.getCenter() + sf::Vector2f(-620, -330));
		stageText.setPosition(view.getCenter() + sf::Vector2f(-620, -290));
		std::string str;
		ss << score;
		str = ss.str();
		ssClear();
		for (int i = int(str.size()); i < 9; i++)
		{
			str = '0' + str;
		}
		str = "Score " + str;
		scoreText.setString(str);
		str.clear();
		ss << timeSec / 60;
		str = ss.str();
		if (timeSec / 60 < 10)
		{
			str = "0" + str;
		}
		ssClear();
		ss << timeSec % 60;
		if (timeSec % 60 < 10)
		{
			str = str + ":0" + ss.str();
		}
		else
		{
			str = str + ":" + ss.str();
		}
		ssClear();
		str = "Time " + str;
		timeText.setString(str);
		str.clear();
		ss << stage.getStage();
		str = ss.str();
		ssClear();
		if (stage.getStage() < 10)
		{
			str = "Stage 0" + str + "/10";
		}
		else
		{
			str = "Stage " + str + "/10";
		}
		stageText.setString(str);
	}
	if (mainMenu || option)
	{
		for (auto& x : notices)
		{
			x.update();
		}
	}
}
void draw()
{
	if (option)
	{
	sf::Text optionText[8], title;
	optionText[0].setString("Main Volume :");
	optionText[0].setPosition(350, 100);
	optionText[1].setString("SE Volume :");
	optionText[1].setPosition(350, 200);
	optionText[2].setString("BGM Volume :");
	optionText[2].setPosition(350, 300);
	optionText[3].setString("Save");
	optionText[3].setPosition(450, 450);
	optionText[4].setString("Quit");
	optionText[4].setPosition(750, 450);
	ss << mainVolume;
	optionText[5].setString(ss.str());
	optionText[5].setPosition(850, 100);
	ssClear();
	ss << SEvolume;
	optionText[6].setString(ss.str());
	optionText[6].setPosition(850, 200);
	ssClear();
	ss << BGMvolume;
	optionText[7].setString(ss.str());
	optionText[7].setPosition(850, 300);
	ssClear();
	for (auto& x : optionText)
	{
		x.setFont(fontEn);
		x.setCharacterSize(60);
		x.setFillColor(sf::Color::White);
		window.draw(x);
	}
	title.setCharacterSize(120);
	title.setFont(fontEn);
	title.setFillColor(sf::Color::White);
	title.setPosition(500, -50);
	title.setString("Options");
	window.draw(title);
	sf::RectangleShape frame(sf::Vector2f(204, 34));
	frame.setFillColor(sf::Color::Transparent);
	frame.setOutlineColor(sf::Color::White);
	frame.setOutlineThickness(2);
	frame.setPosition(620, 130);
	window.draw(frame);
	frame.setPosition(620, 230);
	window.draw(frame);
	frame.setPosition(620, 330);
	window.draw(frame);
	sf::RectangleShape volume;
	volume.setFillColor(sf::Color::White);
	volume.setSize(sf::Vector2f(double(mainVolume) / 100u * 200, 30));
	volume.setPosition(622, 132);
	window.draw(volume);
	volume.setSize(sf::Vector2f(double(SEvolume) / 100u * 200, 30));
	volume.setPosition(622, 232);
	window.draw(volume);
	volume.setSize(sf::Vector2f(double(BGMvolume) / 100u * 200, 30));
	volume.setPosition(622, 332);
	window.draw(volume);
	if (selectY != 3)
	{
		selectCursorSprite.setPosition(290, 120 + selectY * 100);
	}
	else if (selectY == 3)
	{
		selectCursorSprite.setPosition(390 + 300 * selectX, 470);
	}
	window.draw(selectCursorSprite);
	for (auto& x : notices)
	{
		x.draw();
	}
	}
	else if (pauseMenu)
	{
		window.setView(window.getDefaultView());
		sf::Text menuText[5], title;
		title.setCharacterSize(120);
		title.setFillColor(sf::Color::White);
		title.setFont(fontEn);
		title.setString("Pause");
		title.setPosition(550, -50);
		menuText[0].setString("Resume");
		menuText[0].setPosition(500, 150);
		menuText[1].setString("Options");
		menuText[1].setPosition(500, 250);
		menuText[2].setString("Restart");
		menuText[2].setPosition(500, 350);
		menuText[3].setString("Return to Menu");
		menuText[3].setPosition(500, 450);
		menuText[4].setString("Return to Desktop");
		menuText[4].setPosition(500, 550);
		window.draw(title);
		for (auto& x : menuText)
		{
			x.setFont(fontEn);
			x.setFillColor(sf::Color::White);
			x.setCharacterSize(60);
			window.draw(x);
		}
		selectCursorSprite.setPosition(440, 170 + 100 * selectY);
		window.draw(selectCursorSprite);
	}
	else if (inGame)
	{
		window.setView(view);
		//map
		window.draw(mapSprite);
		//player
		playerShip.draw();
		//item
		for (auto& x : items)
		{
			x.draw();
		}
		//enemy
		for (auto& x : enemies)
		{
			x.draw();
		}
		//bullets
		for (auto& x : bullets)
		{
			x.draw();
		}
		//bombs
		for (auto& x : bombs)
		{
			x.Bomb::draw();
		}
		//beams
		for (auto& x : beams)
		{
			x.draw();
		}
		//animation
		for (auto& x : animations)
		{
			x.draw();
		}
		//shield
		if (playerShip.getShield())
		{
			if (playerShip.getShieldBlur())
			{
				shieldSprite.setColor(sf::Color(255, 255, 255, 150));
			}
			else
			{
				shieldSprite.setColor(sf::Color(255, 255, 255, 255));
			}
			window.draw(shieldSprite);
		}
		// debug
		if (debugMode)
		{
			// debug text
			window.draw(debugText);
			//hitbox
			playerShip.drawHitbox(sf::Color::Yellow);
			for (auto& x : enemies)
			{
				x.drawHitbox(sf::Color::Red);
			}
			for (auto& x : bullets)
			{
				x.drawHitbox(sf::Color::Green);
			}
			for (auto& x : bombs)
			{
				x.drawHitbox(sf::Color::Green);
			}
			for (auto& x : beams)
			{
				x.drawHitbox(sf::Color::Red);
			}
			for (auto& x : items)
			{
				x.drawHitbox(sf::Color::Blue);
			}
		}
		// notice
		for (auto& x : notices)
		{
			x.draw();
		}
		//mouse
		window.draw(mouseSprite);
		// UI
		window.draw(uiSprite);
		window.draw(playerHealthBar[1]);
		window.draw(playerHealthBar[0]);
		window.draw(powerBar[1]);
		window.draw(powerBar[0]);
		double percent = double(playerShip.getAmmo()) / playerShip.getMaxAmmo();
		int count = 0;
		if (percent != 1)
		{
			if (percent == 0)
			{
				count = 0;
			}
			else
			{
				count = int(7 * percent) + 1;
			}
		}
		else
		{
			count = 7;
		}
		for (int i = 0; i < playerShip.getMaxAmmo(); i++)
		{
			if (i < playerShip.getAmmo())
			{
				ammoSprite[1].setPosition(uiSprite.getPosition() + sf::Vector2f(45.f + i * 6, 75.f));
				window.draw(ammoSprite[1]);
			}
			else
			{
				ammoSprite[0].setPosition(uiSprite.getPosition() + sf::Vector2f(45.f + i * 6, 75.f));
				window.draw(ammoSprite[0]);
			}
		}
		for (int i = 0; i < playerShip.getMaxBomb(); i++)
		{
			if (i < playerShip.getBomb())
			{
				bombAmmoSprite[1].setPosition(uiSprite.getPosition() + sf::Vector2f(45.f + i * 16, 105.f));
				window.draw(bombAmmoSprite[1]);
			}
			else
			{
				bombAmmoSprite[0].setPosition(uiSprite.getPosition() + sf::Vector2f(45.f + i * 16, 105.f));
				window.draw(bombAmmoSprite[0]);
			}
		}
		// score panel
		window.draw(scorePanelSprite);
		window.draw(scoreText);
		window.draw(timeText);
		window.draw(stageText);
		// minimap
		window.setView(minimap);
		window.draw(minimapBackground);
		mapSprite.setColor(sf::Color(200, 200, 200, 255));
		window.draw(mapSprite);
		mapSprite.setColor(sf::Color(255, 255, 255, 255));
		for (auto& x : enemies)
		{
			if (!x.getExist())
			{
				continue;
			}
			minimapEnemy.setPosition(x.getCoor());
			window.draw(minimapEnemy);
		}
		minimapPlayer.setPosition(playerShip.getCoor());
		window.draw(minimapPlayer);
		minimapViewFrame.setPosition(view.getCenter());
		window.draw(minimapViewFrame);
	}
	else if (specialThanks)
	{
	sf::Text thankText[5], name, title;
	thankText[0].setString("Used Library :");
	thankText[0].setPosition(100, 120);
	thankText[1].setString("Music By :");
	thankText[1].setPosition(100, 270);
	thankText[2].setString("Images By :");
	thankText[2].setPosition(100, 420);
	thankText[3].setString("Game Made By :");
	thankText[3].setPosition(100, 570);
	thankText[4].setString("Back");
	thankText[4].setPosition(1100, 620);
	title.setString("Special Thanks");
	title.setCharacterSize(120);
	title.setFillColor(sf::Color::White);
	title.setFont(fontEn);
	title.setPosition(150, -50);
	window.draw(title);
	for (auto& x : thankText)
	{
		x.setFont(fontEn);
		x.setFillColor(sf::Color::White);
		x.setCharacterSize(60);
		window.draw(x);
	}
	name.setFont(fontCh);
	name.setCharacterSize(60);
	name.setFillColor(sf::Color::White);
	name.setString(L"·s¦Ë°ª¤¤ ¬x´Ë³Í");
	name.setPosition(400, 580);
	window.draw(name);
	window.draw(SFMLSprite);
	window.draw(MODSprite);
	kenneySprite.setPosition(600, 190);
	window.draw(kenneySprite);
	kenneySprite.setPosition(300, 340);
	window.draw(kenneySprite);
	selectCursorSprite.setPosition(1040, 640);
	window.draw(selectCursorSprite);
	}
	else if (mainMenu)
	{
		window.setView(window.getDefaultView());
		sf::Text titleText, startText, optionText, specialThanksText, quitText;
		titleText.setFillColor(sf::Color::White);
		titleText.setCharacterSize(150);
		titleText.setFont(fontEn);
		titleText.setString("Simple\n\tBullethell\n\t\t\t\t\tGame");
		titleText.setPosition(50, 0);
		startText.setFillColor(sf::Color::White);
		startText.setCharacterSize(100);
		startText.setFont(fontEn);
		startText.setString("Start");
		startText.setPosition(750, 350);
		optionText.setFillColor(sf::Color::White);
		optionText.setCharacterSize(100);
		optionText.setFont(fontEn);
		optionText.setString("Options");
		optionText.setPosition(750, 420);
		specialThanksText.setFillColor(sf::Color::White);
		specialThanksText.setCharacterSize(100);
		specialThanksText.setFont(fontEn);
		specialThanksText.setString("Special Thanks");
		specialThanksText.setPosition(750, 490);
		quitText.setFillColor(sf::Color::White);
		quitText.setCharacterSize(100);
		quitText.setFont(fontEn);
		quitText.setString("Quit");
		quitText.setPosition(750, 560);
		window.draw(titleText);
		window.draw(startText);
		window.draw(optionText);
		window.draw(specialThanksText);
		window.draw(quitText);
		selectCursorSprite.setPosition(690, 404 + selectY * 70);
		window.draw(selectCursorSprite);
		for (auto& x : notices)
		{
			x.draw();
		}
	}
	else if (lose)
	{
		sf::Text loseText;
		window.setView(window.getDefaultView());
		loseText.setCharacterSize(120);
		loseText.setFillColor(sf::Color::White);
		loseText.setFont(fontEn);
		loseText.setPosition(100, 150);
		std::string str;
		ss << score;
		ss >> str;
		ssClear();
		for (int i = int(str.size()); i < 9; i++)
		{
			str = '0' + str;
		}
		ss << stage.getStage();
		str = "You Lose...\n\t\tFinal Score " + str;
		str += "\n\t\tStage " + ss.str() + "/10\n";
		ssClear();
		ss << timeSec / 60;
		if (timeSec / 60 < 10)
		{
			str += "\t\tTime 0" + ss.str();
		}
		else
		{
			str += "\t\tTime " + ss.str();
		}
		ssClear();
		ss << timeSec % 60;
		if (timeSec % 60 < 10)
		{
			str += ":0" + ss.str();
		}
		else
		{
			str += ":" + ss.str();
		}
		ssClear();
		str += "\nPress Spacebar to return";
		loseText.setString(str);
		window.draw(loseText);
	}
	else if (win)
	{
		sf::Text winText;
		window.clear(sf::Color(128, 128, 255, 255));
		window.setView(window.getDefaultView());
		winText.setCharacterSize(120);
		winText.setFillColor(sf::Color::White);
		winText.setFont(fontEn);
		winText.setPosition(100, 150);
		std::string str;
		ss << score;
		ss >> str;
		ssClear();
		for (int i = int(str.size()); i < 9; i++)
		{
			str = '0' + str;
		}
		str = "You Win!\n\t\tFinal Score " + str;
		ss << timeSec / 60;
		if (timeSec / 60 < 10)
		{
			str += "\n\t\tTime 0" + ss.str();
		}
		else
		{
			str += "\n\t\tTime " + ss.str();
		}
		ssClear();
		ss << timeSec % 60;
		if (timeSec % 60 < 10)
		{
			str += ":0" + ss.str();
		}
		else
		{
			str += ":" + ss.str();
		}
		ssClear();
		str += "\nPress Spacebar to return";
		winText.setString(str);
		window.draw(winText);
	}
	if (quitWarning)
	{
		sf::Text quitWarningText[3];
		quitWarningText[0].setString("Are you going to exit to\n the desktop?");
		quitWarningText[0].setPosition(430, 250);
		quitWarningText[1].setString("Yes");
		quitWarningText[1].setPosition(480, 420);
		quitWarningText[2].setString("No");
		quitWarningText[2].setPosition(760, 420);
		window.draw(warningFrameSprite);
		for (auto& x : quitWarningText)
		{
			x.setFont(fontEn);
			x.setFillColor(sf::Color::White);
			x.setCharacterSize(60);
			window.draw(x);
		}
		selectCursorSprite.setPosition(410 + 280 * selectX, 444);
		window.draw(selectCursorSprite);
	}
	else if (saveWarning)
	{
		sf::Text saveWarningText[3];
		saveWarningText[0].setString("You haven't save your changes\nDo you want to save them?");
		saveWarningText[0].setPosition(350, 250);
		saveWarningText[1].setString("Save");
		saveWarningText[1].setPosition(440, 420);
		saveWarningText[2].setString("Don't save");
		saveWarningText[2].setPosition(700, 420);
		window.draw(warningFrameSprite);
		for (auto& x : saveWarningText)
		{
			x.setFont(fontEn);
			x.setFillColor(sf::Color::White);
			x.setCharacterSize(60);
			window.draw(x);
		}
		selectCursorSprite.setPosition(380 + 260 * selectX, 444);
		window.draw(selectCursorSprite);
	}
	else if (returnWarning)
	{
		sf::Text returnWarningText[3];
		returnWarningText[0].setString("You will lose all your progress\nDo you want to return to menu?");
		returnWarningText[0].setPosition(340, 250);
		returnWarningText[1].setString("Yes");
		returnWarningText[1].setPosition(480, 420);
		returnWarningText[2].setString("No");
		returnWarningText[2].setPosition(760, 420);
		window.draw(warningFrameSprite);
		for (auto& x : returnWarningText)
		{
			x.setFont(fontEn);
			x.setFillColor(sf::Color::White);
			x.setCharacterSize(60);
			window.draw(x);
		}
		selectCursorSprite.setPosition(410 + 280 * selectX, 444);
		window.draw(selectCursorSprite);
	}
}

void processInput()
{
	if (inGame && !pause)
	{
		// movements
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			playerShip.move(-45);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			playerShip.move(-145);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			playerShip.move(45);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			playerShip.move(145);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			playerShip.move(-90);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			playerShip.move(180);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			playerShip.move(0);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			playerShip.move(90);
		}
		else
		{
			playerShip.move(200);
		}

		// other keys
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
		{
			playerShip.setMaxSpeed(4);
			playerShip.setBrake(0.5);
		}
		else
		{
			playerShip.setMaxSpeed(10);
			playerShip.setBrake(0.7);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
		{
			if (playerShip.getAmmo() != playerShip.getMaxAmmo())
			{
				playerShip.setReloading(true);
			}
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
		{
			if (playerShip.getAmmo() == 0)
			{
				playerShip.setReloading(true);
			}
			else if (!playerShip.getReloading())
			{
				playerShip.shoot();
			}
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
		{
			if (playerShip.getBomb() != 0)
			{
				playerShip.fireBomb();
			}
		}
	}
}
void processEvent(sf::Event event)
{
	switch (event.type)
	{
	case sf::Event::Closed:
		window.close();
		break;
	case sf::Event::KeyPressed:
		if (window.hasFocus())
		{
			if (saveWarning)
			{
				switch (event.key.code)
				{
				default:
					break;
				case sf::Keyboard::A:
					UIswitch.play();
					selectX = 0;
					break;
				case sf::Keyboard::D:
					UIswitch.play();
					selectX = 1;
					break;
				case sf::Keyboard::Space:
					UIclick.play();
					if (selectX == 0)
					{
						osettings.open("resources/settings.txt");
						osettings << "mainVolume " << mainVolume << endl
							<< "SEvolume " << SEvolume << endl
							<< "BGMvolume " << BGMvolume << endl;
						osettings.close();
						notices[noticeCount] = Notice("Changes have been saved!");
						option = false;
						saveWarning = false;
						if (mainMenu)
						{
							selectY = 1;
							selectLimitY = 4;
						}
						else if (pauseMenu)
						{
							selectY = 1;
							selectLimitY = 5;
						}
					}
					else if (selectX == 1)
					{
						saveWarning = false;
						option = false;
						isettings.open("resources/settings.txt");
						std::string str;
						isettings >> str >> str;
						ss << str;
						ss >> mainVolume;
						ssClear();
						isettings >> str >> str;
						ss << str;
						ss >> SEvolume;
						ssClear();
						isettings >> str >> str;
						ss << str;
						ss >> BGMvolume;
						ssClear();
						isettings.close();
						updateVolume();
						if (mainMenu)
						{
							selectY = 1;
							selectLimitY = 4;
						}
						else if (pauseMenu)
						{
							selectY = 1;
							selectLimitY = 5;
						}
					}
					break;
				case sf::Keyboard::Escape:
					UIclick.play();
					saveWarning = false;
					break;
				}
			}
			else if (quitWarning)
			{
				switch (event.key.code)
				{
				default:
					break;
				case sf::Keyboard::A:
					UIswitch.play();
					selectX = 0;
					break;
				case sf::Keyboard::D:
					UIswitch.play();
					selectX = 1;
					break;
				case sf::Keyboard::Space:
					UIclick.play();
					if (selectX == 0)
					{
						window.close();
					}
					else if (selectX == 1)
					{
						quitWarning = false;
					}
				case sf::Keyboard::Escape:
					UIclick.play();
					quitWarning = false;
					break;
				}
			}
			else if (returnWarning)
			{
				switch (event.key.code)
				{
				default:
					break;
				case sf::Keyboard::A:
					UIswitch.play();
					selectX = 0;
					break;
				case sf::Keyboard::D:
					UIswitch.play();
					selectX = 1;
					break;
				case sf::Keyboard::Space:
					UIclick.play();
					if (selectX == 0)
					{
						returnWarning = false;
						inGame = false;
						pause = false;
						pauseMenu = false;
						mainMenu = true;
						battleMusic.stop();
						menuMusic.play();
						for (auto& x : notices)
						{
							x.setExist(false);
						}
						selectY = 0;
						selectLimitY = 4;
					}
					else if (selectX == 1)
					{
						returnWarning = false;
					}
				case sf::Keyboard::Escape:
					UIclick.play();
					returnWarning = false;
					break;
				}
			}
			else if (option)
			{
				switch (event.key.code)
				{
				default:
					break;
				case sf::Keyboard::W:
					UIswitch.play();
					selectY--;
					selectX = 0;
					break;
				case sf::Keyboard::S:
					UIswitch.play();
					selectY++;
					selectX = 0;
					break;
				case sf::Keyboard::A:
				case sf::Keyboard::D:
					if (selectY == 0)
					{
						if (event.key.code == sf::Keyboard::A && mainVolume > 0)
						{
							mainVolume--;
							UIchange.play();
						}
						else if (event.key.code == sf::Keyboard::D && mainVolume < 100)
						{
							mainVolume++;
							UIchange.play();
						}
					}
					else if (selectY == 1)
					{
						if (event.key.code == sf::Keyboard::A && SEvolume > 0)
						{
							SEvolume--;
							UIchange.play();
						}
						else if (event.key.code == sf::Keyboard::D && SEvolume < 100)
						{
							SEvolume++;
							UIchange.play();
						}
					}
					else if (selectY == 2)
					{
						if (event.key.code == sf::Keyboard::A && BGMvolume > 0)
						{
							BGMvolume--;
							UIchange.play();
						}
						else if (event.key.code == sf::Keyboard::D && BGMvolume < 100)
						{
							BGMvolume++;
							UIchange.play();
						}
					}
					else if (selectY == 3)
					{
						if (event.key.code == sf::Keyboard::A)
						{
							UIswitch.play();
							selectX = 0;
						}
						else if (event.key.code == sf::Keyboard::D)
						{
							UIswitch.play();
							selectX = 1;
						}
					}
					updateVolume();
					break;
				case sf::Keyboard::Space:
					if (selectY == 3)
					{
						UIclick.play();
						if (selectX == 0)
						{
							osettings.open("resources/settings.txt");
							osettings << "mainVolume " << mainVolume << endl
											<< "SEvolume " << SEvolume << endl
											<< "BGMvolume " << BGMvolume << endl;
							osettings.close();
							notices[noticeCount] = Notice("Changes had been saved!");
						}
						else if (selectX == 1)
						{
							isettings.open("resources/settings.txt");
							std::string str;
							isettings >> str >> str;
							ss << str;
							int preMainVolume;
							ss >> preMainVolume;
							ssClear();
							isettings >> str >> str;
							ss << str;
							int preSEvolume;
							ss >> preSEvolume;
							ssClear();
							isettings >> str >> str;
							ss << str;
							int preBGMvolume;
							ss >> preBGMvolume;
							ssClear();
							isettings.close();
							if (preMainVolume != mainVolume ||
								preSEvolume != SEvolume ||
								preBGMvolume != BGMvolume)
							{
								saveWarning = true;
								selectX = 0;
							}
							else
							{
								option = false;
								if (mainMenu)
								{
									selectY = 1;
									selectLimitY = 4;
								}
								else if (pauseMenu)
								{
									selectY = 1;
									selectLimitY = 5;
								}
							}
						}
					}
					break;
				case sf::Keyboard::Escape:
					UIclick.play();
					isettings.open("resources/settings.txt");
					std::string str;
					isettings >> str >> str;
					ss << str;
					int preMainVolume;
					ss >> preMainVolume;
					ssClear();
					isettings >> str >> str;
					ss << str;
					int preSEvolume;
					ss >> preSEvolume;
					ssClear();
					isettings >> str >> str;
					ss << str;
					int preBGMvolume;
					ss >> preBGMvolume;
					ssClear();
					isettings.close();
					if (preMainVolume != mainVolume ||
						preSEvolume != SEvolume ||
						preBGMvolume != BGMvolume)
					{
						saveWarning = true;
					}
					else
					{
						option = false;
						if (mainMenu)
						{
							selectY = 1;
							selectLimitY = 4;
						}
						else if (pauseMenu)
						{
							selectY = 1;
							selectLimitY = 5;
						}
					}
					break;
				}
				if (selectLimitX == selectX)
				{
					selectX = 0;
				}
				else if (selectX == -1)
				{
					selectX = selectLimitX - 1;
				}
				if (selectY == selectLimitY)
				{
					selectY = 0;
				}
				else if (selectY == -1)
				{
					selectY = selectLimitY - 1;
				}
			}
			else if (pauseMenu)
			{
				switch (event.key.code)
				{
				default:
					break;
				case sf::Keyboard::W:
					UIswitch.play();
					selectY--;
					break;
				case sf::Keyboard::S:
					UIswitch.play();
					selectY++;
					break;
				case sf::Keyboard::Space:
					UIclick.play();
					switch (selectY)
					{
					case 0:
						pause = false;
						pauseMenu = false;
						selectLimitY = 4;
						break;
					case 1:
						option = true;
						selectY = 0;
						selectLimitY = 4;
						break;
					case 2:
						gameInitialize();
						pause = false;
						pauseMenu = false;
						break;
					case 3:
						returnWarning = true;
						selectX = 0;
						break;
					case 4:
						quitWarning = true;
						selectX = 0;
						break;
					}
					break;
				case sf::Keyboard::Escape:
					UIclick.play();
					pause = false;
					pauseMenu = false;
					selectLimitY = 4;
					break;
				}
				if (selectY == selectLimitY)
				{
					selectY = 0;
				}
				else if (selectY == -1)
				{
					selectY = selectLimitY - 1;
				}
			}
			else if (inGame)
			{
				switch (event.key.code)
				{
				default:
					break;
				case sf::Keyboard::Escape:
					pause = true;
					pauseMenu = true;
					selectY = 0;
					selectLimitY = 5;
					break;
				case sf::Keyboard::F3:
					if (debugMode)
					{
						debugMode = 0;
					}
					else
					{
						debugMode = 1;
					}
					break;
				}
			}
			else if (specialThanks)
			{
				switch (event.key.code)
				{
				default:
					break;
				case sf::Keyboard::Space:
				case sf::Keyboard::Escape:
					UIclick.play();
					specialThanks = false;
					break;
				}
			}
			else if (mainMenu)
			{
				switch (event.key.code)
				{
				default:
					break;
				case sf::Keyboard::W:
					UIswitch.play();
					selectY--;
					break;
				case sf::Keyboard::S:
					UIswitch.play();
					selectY++;
					break;
				case sf::Keyboard::Space:
					UIclick.play();
					switch (selectY)
					{
					case 0:
						inGame = true;
						mainMenu = false;
						gameInitialize();
						menuMusic.stop();
						break;
					case 1:
						option = true;
						selectY = 0;
						selectLimitY = 4;
						break;
					case 2:
						specialThanks = true;
						break;
					case 3:
						quitWarning = true;
						selectX = 0;
						break;
					}
					break;
				case sf::Keyboard::Escape:
					UIclick.play();
					quitWarning = true;
					selectX = 0;
					break;
				case sf::Keyboard::F12:
					window.close();
					mapEditor();
					break;
				}
				if (selectY == selectLimitY)
				{
					selectY = 0;
				}
				else if (selectY == -1)
				{
					selectY = selectLimitY - 1;
				}
			}
			else if (lose)
			{
				if (event.key.code == sf::Keyboard::Space)
				{
					mainMenu = true;
					lose = false;
					gameoverMusic.stop();
					menuMusic.play();
				}
			}
			else if (win)
			{
				if (event.key.code == sf::Keyboard::Space)
				{
					mainMenu = true;
					win = false;
					winMusic.stop();
					menuMusic.play();
				}
			}
		}
	}
}

void updateVolume()
{
	double main = double(mainVolume) / 100;
	double se = double(SEvolume) / 100;
	double bgm = double(BGMvolume) / 100;
	// sound effect
	playerShoot.setVolume(20 * main * se);
	enemyShoot.setVolume(15 * main * se);
	getHealth.setVolume(10 * main * se);
	getPower.setVolume(10 * main * se);
	getBomb.setVolume(30 * main * se);
	getShield.setVolume(40 * main * se);
	playerHit.setVolume(20 * main * se);
	enemyHit.setVolume(7 * main * se);
	playerDie.setVolume(10 * main * se);
	enemyDie.setVolume(20 * main * se);
	reload.setVolume(100 * main * se);
	shieldBreak.setVolume(60 * main * se);
	UIback.setVolume(10 * main * se);
	UIclick.setVolume(50 * main * se);
	UIswitch.setVolume(30 * main * se);
	UIchange.setVolume(10 * main * se);
	stageClear.setVolume(20 * main * se);
	powerUp.setVolume(10 * main * se);
	beamShoot.setVolume(30 * main * se);
	beamStart.setVolume(20 * main * se);
	bombHit.setVolume(60 * main * se);
	// music
	menuMusic.setVolume(10 * main * bgm);
	battleMusic.setVolume(10 * main * bgm);
	gameoverMusic.setVolume(10 * main * bgm);
	winMusic.setVolume(10 * main * bgm);
}