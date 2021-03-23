#include "Animation.h"

Animation::Animation(sf::Texture* Texture, sf::Vector2u sprites, float switchTime) {
    this->X = 0;
    this->Y = 0;
    this->frames = 0;
    this->totalframes = 0;
    this->spritesPixel.width = Texture->getSize().x / float(sprites.x);
    this->spritesPixel.height = Texture->getSize().y / float(sprites.y);
    this->sprites.x = sprites.x;
    this->sprites.y = sprites.y;
    this->Picture.setTexture(*Texture);
    this->switchTime = switchTime;
    this->totalTime = 0.f;
}

bool Animation::Update(float dt) {
    this->totalTime += dt;
    frames = this->totalTime / switchTime;
    this->totalframes += frames;

    this->totalTime -= frames * switchTime;
    this->X += frames;
     
    if (this->X >= sprites.x) {
        this->X -= sprites.x;
        if (this->Y <= sprites.y)
            this->Y++;
        else
            return false;
    }
    this->Picture.setTextureRect({ this->X * spritesPixel.width, this->Y * spritesPixel.height,spritesPixel.width,spritesPixel.height });
    return true;
}