#pragma once
#include <SFML/Graphics.hpp>
class Ship {
public:
	sf::Vector2f Velocity;
	sf::Vector2f direction;
	sf::Vector2f position;
	float acceleration = 1.f;
	float drag = 0.f;
	sf::Sprite Picture;
	float maxVelocity = 5.f;
public:
	Ship(sf::Texture* Texture, float acceleration, float drag);
	void Update(sf::RenderWindow* window, float dtf);
};