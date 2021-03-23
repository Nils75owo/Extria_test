#include "Ship.h"
#include <iostream>

Ship::Ship(sf::Texture* Texture, float acceleration, float drag) {
	this->Picture.setTexture(*Texture);
	this->Velocity = sf::Vector2f(0.f, 0.f);
	this->direction = sf::Vector2f(0.f, 0.f);
	this->position = sf::Vector2f(0.f, 0.f);
	this->acceleration = acceleration;
	this->drag = drag;
}

void Ship::Update(sf::RenderWindow *window, float dtf) {
	this->position = this->Picture.getPosition();

	if (this->Velocity.x > 0.f) {
		this->Velocity.x -= this->drag;
		if (this->Velocity.x < 0.f)
			this->Velocity.x = 0.f;
	}
	else if (this->Velocity.x < 0.f) {
		this->Velocity.x += drag;
		if (this->Velocity.x > 0.f)
			this->Velocity.x = 0.f;
	}
	if (this->Velocity.y > 0.f) {
		this->Velocity.y -= this->drag;
		if (this->Velocity.y < 0.f)
			this->Velocity.y = 0.f;
	}
	else if (this->Velocity.y < 0.f) {
		this->Velocity.y += drag;
		if (this->Velocity.y > 0.f)
			this->Velocity.y = 0.f;
	}
	if (this->Velocity.y > 0.1f)
		if (window->getSize().y < this->Picture.getPosition().y + 32) this->Velocity.y = 0;
	if (this->Velocity.y < 0.f)
		if (0.f > this->Picture.getPosition().y) this->Velocity.y = 0;
	if (this->Velocity.x > 0.1f)
		if (window->getSize().x < this->Picture.getPosition().x + 64) this->Velocity.x = 0;
	if (this->Velocity.x < 0.f)
		if (0.f > this->Picture.getPosition().x) this->Velocity.x = 0;
	this->Picture.move(this->Velocity.x * dtf, this->Velocity.y * dtf);
	//this->Picture.setPosition(round(this->Picture.getPosition().x), round(this->Picture.getPosition().y));
}