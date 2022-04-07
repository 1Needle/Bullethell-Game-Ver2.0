#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <cmath>
#include <string>
#include <sstream>
#include <fstream>
#include "Character.h"
#include "Bullet.h"
#include "Animation.h"
#include "Item.h"
#include "Stage.h"
#include "Notice.h"

double radian(double);
double degree(double);
double faceToward(sf::Vector2f origin, sf::Vector2f target);
int findNearestEnemy(sf::Vector2f coor, double direction, double angle);
int findNearestEnemy(sf::Vector2f);
double tracking(sf::Vector2f origin, sf::Vector2f target, double direction, double turnSpeed);
void ssClear();
bool isCollided(Character character, Bullet bullet);
bool isCollided(Character c1, Character c2);

int mapEditor();
void initialize();
void update();
void draw();
void processInput();
void processEvent(sf::Event event);
void updateVolume();