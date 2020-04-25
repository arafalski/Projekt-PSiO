#ifndef PROJEKT_PACMAN_HPP
#define PROJEKT_PACMAN_HPP

#include <SFML/Graphics.hpp>
#include "character.hpp"
#include <cmath>

class Pacman : public Character {
public:
    Pacman(float speed);

    void update(float deltaTime, sf::Vector2f targetPosition);
};


#endif //PROJEKT_PACMAN_HPP
