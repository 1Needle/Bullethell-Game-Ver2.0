#include <SFML/Graphics.hpp>
#include "Character.h"
#include "Function.h"
#include "Bullet.h"
#include "Hitbox.h"
#include "Line.h"
#include <iostream>

extern sf::Vector2i mapSize;
sf::RenderWindow window(sf::VideoMode(1280, 720), "Bullethell Game", sf::Style::Default);
sf::RectangleShape spaceDustSprite[3];
sf::Texture shipTexture, tileTexture, characterTexture;
sf::Sprite shipSprite[3], bulletSprite[25], enemySprite[18];
Player playerShip(100, 0, 3, sf::Vector2f(mapSize.x / 2, mapSize.y / 2), 15, 1, 5, 0.7);
Enemy enemies[100];
Bullet bullets[2000];
int enemyCount = 0;
int bulletCount = 0;
long long int frametime = 0;
bool debugMode = 0;
bool pause = false;
bool mainMenu = true, inGame = false, option = false, specialThanks = false, quitWarning = false, saveWarning = false, pauseMenu = false, returnWarning = false;
extern bool lose, win;
sf::Font fontEn, fontCh;
sf::Text debugText;

int main()
{
    //initialize
    initialize();
    //main loop
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event)) // event control
        {
            processEvent(event);
        }
        processInput();
        update();
        window.clear();
        draw();
        window.display();
    }
    return 0;
}