#include "E_Ship.h"

E_Ship::E_Ship(sf::Texture* Texture) {
	this->Picture.setTexture(*Texture);
	this->time = 0;
}

bool E_Ship::Update(float dt, float e_Shotpawn) {
	this->time += dt;
	//std::cout << "dt: " << dt << std::endl;
	std::cout << "time: " << this->time << std::endl;
	if (this->time > e_Shotpawn) {
		this->time -= e_Shotpawn;
		std::cout << "----------------------------------\n";
		return true;
	}
	std::cout << "after\n";
	return false;
}