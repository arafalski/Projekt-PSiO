#include "pacman.hpp"

Pacman::Pacman(sf::Texture *pacmanTexture, float speed) {
    this->speed = speed;

    setSize(sf::Vector2f(40.0f, 40.0f));
    setOrigin(getSize().x / 2, getSize().y / 2);
    setTexture(pacmanTexture);
}

void Pacman::update(float deltaTime, sf::Vector2f targetPosition) {
    sf::Vector2f distance = targetPosition - getPosition();

    velocity.y = sqrtf(
            (speed * powf(distance.y, 2.0f)) / (powf(distance.x, 2.0f) + powf(distance.y, 2.0f)));
    velocity.x = (std::abs(distance.x) * velocity.y) / std::abs(distance.y);

    if (distance.x < 0) {
        velocity.x *= -1;
    }

    if (distance.y < 0) {
        velocity.y *= -1;
    }

    auto angle = static_cast<float>(atan2f(distance.x, -distance.y) * 180 / M_PI) - 90;
    move(velocity * deltaTime);
    setRotation(angle);
}
