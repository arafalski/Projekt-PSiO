#ifndef PROJEKT_CHARACTER_HPP
#define PROJEKT_CHARACTER_HPP

#include <SFML/Graphics.hpp>
#include "animation.hpp"

class Character : public sf::RectangleShape {
protected:
    float speed;
    sf::Vector2f velocity;
    Animation animation;
    unsigned int rowOfTexture = 0;
public:
    Character(sf::Texture *texture, sf::Vector2u imageCount, float switchTime, float speed);

    virtual void update(float deltaTime, sf::Vector2f targetPosition) = 0;
};


#endif //PROJEKT_CHARACTER_HPP
