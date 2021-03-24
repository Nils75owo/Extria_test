#include "E_Ship.h"

E_Ship::E_Ship(sf::Texture* Texture) {
	this->Picture.setTexture(*Texture);
	this->time = 0;
}

bool E_Ship::Update(float *dt, int* e_Shotpawn) {
	this->time += int(*dt);
	if (time > *e_Shotpawn) {
		time = 0;
		return true;
	}
	return false;
}