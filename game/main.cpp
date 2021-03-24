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
#include "E_Ship.h"

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
    int e_Ship_spawntime = 1000;
    int dedexplosions = 10;
    int e_Ship_Shotpawn = 700;
    float shipacceleration = 1.f;
    float shipdrag = 0.2f;

    //variables
    sf::Clock enemytime;
    sf::Clock randomexplosion;
    sf::Clock pressedtime;
    sf::Clock e_ship;
    bool previousKeyState = true;
    bool lost = false;
    bool first = true;
    int oldenemycount;
    int IexplosionX = 0;
    int IexplosionY = 0;
    int random;
    int i = 0;
    int score = 0;
    float f60 = 0.016;

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

    //e_Ship shot
    sf::Texture e_Ship_shotTexture;
    sf::Sprite e_Ship_shot;
    if (!e_Ship_shotTexture.loadFromFile("Textures/e_Ship_shot.png"))
        std::cout << "Could not load e_Ship_shotTexture.png" << std::endl;
    e_Ship_shot.setTexture(e_Ship_shotTexture);
    std::vector<sf::Sprite> e_Ship_shots;

    //ship
    sf::Texture shipTexture;
    //sf::Sprite ship;
    if (!shipTexture.loadFromFile("Textures/ship.png"))
        std::cout << "Could not load ship.png" << std::endl;
    //ship.setTexture(shipTexture);
    //std::vector<sf::Sprite> ships;
    Ship ship(&shipTexture, shipacceleration, shipdrag);
    ship.Picture.setPosition(1.f, 1.f);

    //e_Ship
    sf::Texture e_ShipTexture;
    sf::Sprite e_Ship_Sprite;
    if (!e_ShipTexture.loadFromFile("Textures/e_Ship.png"))
        std::cout << "Could not load e_Ship.png" << std::endl;
    e_Ship_Sprite.setTexture(e_ShipTexture);
    E_Ship e_Ship(&e_ShipTexture);
    std::vector<E_Ship> e_Ships;

    //explosion
    sf::Texture explosionTexture;
    if (!explosionTexture.loadFromFile("Textures/explosion.png"))
        std::cout << "Could not load explosion.png" << std::endl;
    std::vector<Animation> explosions;
    Animation explosion(&explosionTexture, sf::Vector2u(8, 6), f60);
    explosion.Update(&f60);

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

            if (enemytime.getElapsedTime().asMilliseconds() > e_Ship_spawntime) { //spawn enemies
                e_Ship.Picture.setPosition(window.getSize().x, (rand() % int(window.getSize().y - 40)));
                e_Ships.push_back(e_Ship);
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

            for (size_t i = 0; i < e_Ship_shots.size(); i++) { //move e_Ship shots
                e_Ship_shots[i].move(15.f * dtf, 0.f);
            }

            for (size_t i = 0; i < e_Ships.size(); i++) { //move e_Ships
                e_Ships[i].Picture.move(-5.f * dtf, 0.f);
            }
            for (size_t i = 0; i < e_Ships.size(); i++) { //spawn e_Ship shots
                if (e_Ships[i].Update(&dt, &e_Ship_Shotpawn)) {
                    e_Ship_shot.setPosition(e_Ships[i].Picture.getPosition().x, e_Ships[i].Picture.getPosition().y + 32);
                    e_Ship_shots.push_back(e_Ship_shot);
                    SoundShot.play();
                }
            }

            for (size_t i = 0; i < e_Ships.size(); i++) {
                oldenemycount = e_Ships.size();
                for (size_t k = 0; k < shots.size(); k++) {
                    if (shots[k].getGlobalBounds().intersects(e_Ships[i].Picture.getGlobalBounds())) {
                        explosion.Picture.setPosition(e_Ships[i].Picture.getPosition().x, e_Ships[i].Picture.getPosition().y);
                        explosions.push_back(explosion);
                        shots.erase(shots.begin() + k);
                        //std::cout << "shot deleted\n";
                        e_Ships.erase(e_Ships.begin() + i);
                        //std::cout << "enemy deleted\n";
                        SoundExplosion.play();
                        score++;
                        break;
                    }
                }
                if (oldenemycount == e_Ships.size()) {
                    if (ship.Picture.getGlobalBounds().intersects(e_Ships[i].Picture.getGlobalBounds())) {
                        explosion.Picture.setPosition(e_Ships[i].Picture.getPosition().x, e_Ships[i].Picture.getPosition().y);
                        explosions.push_back(explosion);

                        e_Ships.erase(e_Ships.begin() + i);
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

        for (size_t i = 0; i < e_Ships.size(); i++) {
            e_Ships[i].Update(&dt, &e_Ship_Shotpawn);
        }
        //std::cout << "X: " << ship.Picture.getPosition().x << std::endl;
        //std::cout << "Y: " << ship.Picture.getPosition().y << std::endl;

        for (size_t i = 0; i < explosions.size(); i++) { //update explosions
            if (explosions[i].Update(&dt)) {
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
                e_Ships.clear();
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

        for (size_t i = 0; i < e_Ship_shots.size(); i++) { //draw e_Ship shots
            if (e_Ship_shots[i].getPosition().x <= window.getSize().x + 50 && e_Ship_shots[i].getPosition().y <= window.getSize().y && e_Ship_shots[i].getPosition().x > -50.f && e_Ship_shots[i].getPosition().y > 0.f)
                window.draw(e_Ship_shots[i]);
            else {
                e_Ship_shots.erase(e_Ship_shots.begin() + i); //erase if out of screen
            }
        }

        for (size_t i = 0; i < e_Ships.size(); i++) { //draw e_Ship
            if (e_Ships[i].Picture.getPosition().x <= window.getSize().x + 50 && e_Ships[i].Picture.getPosition().y <= window.getSize().y && e_Ships[i].Picture.getPosition().x > -50.f && e_Ships[i].Picture.getPosition().y > 0.f)
                window.draw(e_Ships[i].Picture);
            else {
                e_Ships.erase(e_Ships.begin() + i); //erase if out of screen
            }
        }

        for (size_t i = 0; i < shots.size(); i++) { //draw e_Ship shots
            if (shots[i].getPosition().x <= window.getSize().x && shots[i].getPosition().y <= window.getSize().y && shots[i].getPosition().x > 0.f && shots[i].getPosition().y > 0.f)
                window.draw(shots[i]);
            else {
                shots.erase(shots.begin() + i); //erase if out of screen
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
        //std::cout << e_Ships.size() << "\n"; //enemies on screen
        //std::cout << pressedtime << endl; //pressed timer

        //for (size_t i = 0; i < explosions.size(); i++) { //display explosion data
        //    std::cout << explosionsX[i] << endl;
        //    std::cout << explosionsY[i] << endl;
        //}

        //std::cout << "dt: " << dt << std::endl;
    }
    return 0;
}