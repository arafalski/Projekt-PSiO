#ifndef PROJEKT_PACMAN_HPP
#define PROJEKT_PACMAN_HPP

#include <SFML/Graphics.hpp>
#include <cmath>
#include "animation.hpp"
#include "consts.hpp"
#include "collider.hpp"

class Pacman : public sf::RectangleShape {
    float speed;
    sf::Vector2f velocity;
    Animation animation;
    unsigned int rowOfTexture = 0;
public:
    Pacman(sf::Texture *pacmanTexture, const sf::Vector2u &imageCount, const float &switchTime, const float &speed);

    ~Pacman() override = default;

    void update(const float &deltaTime, const sf::Vector2f &targetPosition);

    void onCollision(const sf::Vector2f &direction);

    Collider getCollider();
};


#endif //PROJEKT_PACMAN_HPP
