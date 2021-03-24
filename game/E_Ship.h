#pragma once
#include <SFML/Graphics.hpp>
class E_Ship {
public:
	sf::Sprite Picture;
private:
	int time;
public:
	E_Ship(sf::Texture* Texture);
	bool Update(float *dt, int* e_Shotpawn);
};