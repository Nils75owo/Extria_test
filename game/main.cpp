#include <iostream>
#include "SFML\Graphics.hpp"
#include "SFML\Window.hpp"
#include "SFML\System.hpp"
#include "SFML\Audio.hpp"
#include "SFML\Network.hpp"
#include <cstdlib>
#include <algorithm>
#include <random>
#include "Animation.h"
#include "Ship.h"

int main()
{
    srand((unsigned int)time(NULL));

    sf::RenderWindow window(sf::VideoMode(1280, 720), "UwU");
    window.setFramerateLimit(60);

    sf::Clock clock;
    float dt;
    float f = 60.f; //normal frames
    float dtf;

    //settings
    int shotcooldown = 10;
    int autoshottime = 700;
    int enemyspawntime = 500;
    int dedexplosions = 10;
    float shipacceleration = 1.f;
    float shipdrag = 0.2f;

    //variables
    bool previousKeyState = true;
    sf::Clock enemytime;
    sf::Clock randomexplosion;
    sf::Clock pressedtime;
    int oldenemycount;
    bool lost = false;
    int IexplosionX = 0;
    int IexplosionY = 0;
    int random;
    bool first = true;
    int i = 0;
    int score = 0;

    //create

    //background
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("Textures/background.png"))
        std::cout << "Could not load background.png" << std::endl;
    sf::Sprite background(backgroundTexture);

    //shot
    sf::Texture shotTexture;
    sf::Sprite shot;
    if (!shotTexture.loadFromFile("Textures/shot.png"))
        std::cout << "Could not load shot.png" << std::endl;
    shot.setTexture(shotTexture);
    std::vector<sf::Sprite> shots;
    int shottimer = 0;

    //ship
    sf::Texture shipTexture;
    //sf::Sprite ship;
    if (!shipTexture.loadFromFile("Textures/ship.png"))
        std::cout << "Could not load ship.png" << std::endl;
    //ship.setTexture(shipTexture);
    //std::vector<sf::Sprite> ships;
    Ship ship(&shipTexture, shipacceleration, shipdrag);
    ship.Picture.setPosition(1.f, 1.f);

    //enemy
    sf::Texture enemyTexture;
    sf::Sprite enemy;
    if (!enemyTexture.loadFromFile("Textures/enemy.png"))
        std::cout << "Could not load enemy.png" << std::endl;
    enemy.setTexture(enemyTexture);
    std::vector<sf::Sprite> enemies;

    //explosion
    sf::Texture explosionTexture;
    if (!explosionTexture.loadFromFile("Textures/explosion.png"))
        std::cout << "Could not load explosion.png" << std::endl;
    std::vector<Animation> explosions;
    Animation explosion(&explosionTexture, sf::Vector2u(8, 6), 0.016f);
    explosion.Update(0.016f);

    //shotSound
    sf::SoundBuffer SoundBShot;
    if (!SoundBShot.loadFromFile("Sounds/shot.wav"))
        std::cout << "Can not load shot.wav" << std::endl;
    sf::Sound SoundShot;
    SoundShot.setBuffer(SoundBShot);
    SoundShot.setVolume(25.f);

    //explosionSound
    sf::SoundBuffer SoundBExplosion;
    if (!SoundBExplosion.loadFromFile("Sounds/explosion.wav"))
        std::cout << "Can not load explosion.wav" << std::endl;
    sf::Sound SoundExplosion;
    SoundExplosion.setBuffer(SoundBExplosion);
    SoundExplosion.setVolume(25.f);

    while (window.isOpen())
    {

        sf::Event event; //events
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        dt = clock.restart().asSeconds();
        dtf = dt * f;

        if (!lost) {

            //controls
            ship.direction = sf::Vector2f(0.f, 0.f);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) //cls
                system("cls");
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) { //down | move down
                ship.direction.y = 1.f;
                if (ship.Velocity.y < ship.maxVelocity)
                    ship.Velocity.y += ship.acceleration * ship.direction.y;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) { //up | move up
                ship.direction.y = -1.f;
                if (ship.Velocity.y > -ship.maxVelocity)
                    ship.Velocity.y += ship.acceleration * ship.direction.y;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) { //right | move right
                ship.direction.x = 1.f;
                if (ship.Velocity.x < ship.maxVelocity)
                    ship.Velocity.x += ship.acceleration * ship.direction.x;
                }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) { //left | move left
                ship.direction.x = -1.f;
                if (ship.Velocity.x > -ship.maxVelocity)
                    ship.Velocity.x += ship.acceleration * ship.direction.x;
                }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) //space | shoot
            {
                if (pressedtime.getElapsedTime().asMilliseconds() < autoshottime) {
                    if (!previousKeyState) {
                        shot.setPosition(ship.Picture.getPosition().x + 55, ship.Picture.getPosition().y + 20.f); //normal shoot
                        shots.push_back(sf::Sprite(shot));
                        SoundShot.play();
                        shottimer = 0;
                    }
                }
                else {
                    if (shottimer >= shotcooldown) {   //auto shoot
                        shot.setPosition(ship.Picture.getPosition().x + 55, ship.Picture.getPosition().y + 20.f);
                        shots.push_back(sf::Sprite(shot));
                        SoundShot.play();
                        shottimer = 0;
                    }
                }
            }

            if (enemytime.getElapsedTime().asMilliseconds() > enemyspawntime) { //spawn enemies
                enemy.setPosition(window.getSize().x, (rand() % int(window.getSize().y - 40)));
                enemies.push_back(enemy);
                enemytime.restart();
            }

            previousKeyState = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
            if (!previousKeyState) //reset pressedtime
                pressedtime.restart();

            if (shottimer <= shotcooldown) //shottimer + 1
                shottimer++;

            for (size_t i = 0; i < shots.size(); i++) { //move shots
                shots[i].move(15.f * dtf, 0.f);
            }

            for (size_t i = 0; i < enemies.size(); i++) { //move enemies
                enemies[i].move(-5.f * dtf, 0.f);
            }

            for (size_t i = 0; i < enemies.size(); i++) {
                oldenemycount = enemies.size();
                for (size_t k = 0; k < shots.size(); k++) {
                    if (shots[k].getGlobalBounds().intersects(enemies[i].getGlobalBounds())) {
                        explosion.Picture.setPosition(enemies[i].getPosition().x, enemies[i].getPosition().y);
                        explosions.push_back(explosion);
                        shots.erase(shots.begin() + k);
                        //std::cout << "shot deleted\n";
                        enemies.erase(enemies.begin() + i);
                        //std::cout << "enemy deleted\n";
                        SoundExplosion.play();
                        score++;
                        break;
                    }
                }
                if (oldenemycount == enemies.size()) {
                    if (ship.Picture.getGlobalBounds().intersects(enemies[i].getGlobalBounds())) {
                        explosion.Picture.setPosition(enemies[i].getPosition().x, enemies[i].getPosition().y);
                        explosions.push_back(explosion);

                        enemies.erase(enemies.begin() + i);
                        //std::cout << "enemy deleted\n";
                        SoundExplosion.play();
                        for (int i = 1; i < 15; i++) {
                            explosion.Picture.setPosition(ship.Picture.getPosition().x - 64 + (rand() % -50 + 50), ship.Picture.getPosition().y - 64 + (rand() % -50 + 50));
                            explosions.push_back(explosion);
                        }
                        lost = true;
                        break;
                    }
                }
            }
        }

        ship.Update(&window, dtf);
        //std::cout << "X: " << ship.Picture.getPosition().x << std::endl;
        //std::cout << "Y: " << ship.Picture.getPosition().y << std::endl;

        for (size_t i = 0; i < explosions.size(); i++) { //update explosions
            if (explosions[i].Update(dt)) {
            //std::cout << i << std::endl;
            //std::cout << "X: " << explosions[i].X << std::endl;
            //std::cout << "Y: " << explosions[i].Y << std::endl;
            }
            else {
                explosions.erase(explosions.begin() + i);
            }
        }

        if (lost) {
            if (first) {
                randomexplosion.restart();
                random = (150 + (rand() % 250 + 1));
                i = dedexplosions;
                first = false;
            }
            if (i > 0) {
                int time = randomexplosion.getElapsedTime().asMilliseconds();
                if (time > random) {
                    random = (150 + (rand() % 250 + 1));
                    SoundExplosion.play();
                    randomexplosion.restart();
                    explosion.Picture.setPosition(ship.Picture.getPosition().x- 64 + (rand() % -40 + 40), ship.Picture.getPosition().y - 64 + (rand() % 40 + 40));
                    explosions.push_back(explosion);
                    i--;
                }
            }
            else {
                explosions.clear();
                enemies.clear();
                shots.clear();
                ship.Picture.setPosition(1.f, 1.f);
                lost = false;
                first = true;
                score = 0;
            }
        }

        //draw
        window.clear();

        window.draw(background);

        for (size_t i = 0; i < shots.size(); i++) { //draw shots
            if (shots[i].getPosition().x <= window.getSize().x && shots[i].getPosition().y <= window.getSize().y && shots[i].getPosition().x > 0.f && shots[i].getPosition().y > 0.f)
                window.draw(shots[i]);
            else {
                shots.erase(shots.begin() + i); //erase if out of screen
            }
        }

        for (size_t i = 0; i < enemies.size(); i++) { //draw enemy
            if (enemies[i].getPosition().x <= window.getSize().x + 50 && enemies[i].getPosition().y <= window.getSize().y && enemies[i].getPosition().x > -50.f && enemies[i].getPosition().y > 0.f)
                window.draw(enemies[i]);
            else {
                enemies.erase(enemies.begin() + i); //erase if out of screen
            }
        }

        window.draw(ship.Picture);
        //ship.Update(&window);

        for (size_t i = 0; i < explosions.size(); i++) { //draw explosions
            window.draw(explosions[i].Picture);
            //std::cout << "X: " << explosions[i].Picture.getPosition().x << std::endl;
            //std::cout << "Y: " << explosions[i].Picture.getPosition().y << std::endl;
        }

        window.display();

        //debug

        //std::cout << shots.size() << "\n"; //shots on screen
        //std::cout << enemies.size() << "\n"; //enemies on screen
        //std::cout << pressedtime << endl; //pressed timer

        //for (size_t i = 0; i < explosions.size(); i++) { //display explosion data
        //    std::cout << explosionsX[i] << endl;
        //    std::cout << explosionsY[i] << endl;
        //}

        //std::cout << "dt: " << dt << std::endl;
    }
    return 0;
}