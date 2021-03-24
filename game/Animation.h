#pragma once
#include <SFML/Graphics.hpp>
class Animation {
public:
    Animation(sf::Texture* Texture, sf::Vector2u sprites, float switchTime);
    bool Update(float *dt);
public:
    sf::Sprite Picture;
private:
    int X;
    int Y;
    int frames;
    int totalframes;
    float totalTime;
    float switchTime;
    sf::IntRect spritesPixel;
    sf::Vector2u sprites;
};