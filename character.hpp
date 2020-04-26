#ifndef PROJEKT_CHARACTER_HPP
#define PROJEKT_CHARACTER_HPP

#include <SFML/Graphics.hpp>

class Character : public sf::RectangleShape {
protected:
    float speed;
    sf::Vector2f velocity;
public:
    virtual void update(float deltaTime, sf::Vector2f targetPosition) = 0;
};


#endif //PROJEKT_CHARACTER_HPP
