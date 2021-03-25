#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
class E_Ship {
public:
	sf::Sprite Picture;
private:
	float time;
public:
	E_Ship(sf::Texture* Texture);
	bool Update(float dt, float e_Shotpawn);
};