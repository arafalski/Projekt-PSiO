#ifndef PROJEKT_PACMAN_HPP
#define PROJEKT_PACMAN_HPP

#include <SFML/Graphics.hpp>
#include "character.hpp"
#include <cmath>

class Pacman : public Character {
public:
    Pacman(sf::Texture *pacmanTexture, sf::Vector2u imageCount, float switchTime, float speed);
    ~Pacman() override = default;

    void update(float deltaTime, sf::Vector2f targetPosition) final;
};


#endif //PROJEKT_PACMAN_HPP
