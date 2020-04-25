#ifndef PROJEKT_CHARACTER_HPP
#define PROJEKT_CHARACTER_HPP

#include <SFML/Graphics.hpp>

class Character {
protected:
    sf::RectangleShape body;
    float speed;
    sf::Vector2f velocity;
public:
    virtual void update(float deltaTime, sf::Vector2f targetPosition) = 0;

    void draw(sf::RenderWindow &window);

    sf::Vector2f getPosition();
};


#endif //PROJEKT_CHARACTER_HPP
