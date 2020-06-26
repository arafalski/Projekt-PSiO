#ifndef PROJEKT_COLLIDABLE_HPP
#define PROJEKT_COLLIDABLE_HPP

#include <SFML/Graphics.hpp>
#include "consts.hpp"

class Collidable : public sf::RectangleShape {
public:
    bool checkCollision(Collidable& other, Direction& dir);
};

#endif //PROJEKT_COLLIDABLE_HPP
