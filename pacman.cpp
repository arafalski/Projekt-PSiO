#include "pacman.hpp"

Pacman::Pacman(sf::Texture *pacmanTexture, sf::Vector2u imageCount, float switchTime, float speed)
        : Character(pacmanTexture, imageCount, switchTime, speed) {
    setSize(sf::Vector2f(40.0f, 40.0f));
    setOrigin(getSize() / 2.0f);
    setTexture(pacmanTexture);
}

void Pacman::update(float deltaTime, sf::Vector2f targetPosition) {
    sf::Vector2f distance = targetPosition - getPosition();

    if (distance.x != 0 && distance.y != 0) {
        velocity.y = sqrtf(
                (powf(speed, 2.0f) * powf(distance.y, 2.0f)) / (powf(distance.x, 2.0f) + powf(distance.y, 2.0f)));
        velocity.x = (std::abs(distance.x) * velocity.y) / std::abs(distance.y);
    } else if (distance.x != 0) {
        velocity.y = 0;
        velocity.x = speed;
    } else if (distance.y != 0) {
        velocity.y = speed;
        velocity.x = 0;
    } else {
        velocity.y = 0;
        velocity.x = 0;
    }

    if (distance.x < 0) {
        velocity.x *= -1;
    }

    if (distance.y < 0) {
        velocity.y *= -1;
    }

    auto angle = static_cast<float>(atan2f(distance.x, -distance.y) * 180 / M_PI) - 90.0f;
    move(velocity * deltaTime);
    setRotation(angle);

    animation.update(deltaTime);
    setTextureRect(animation.uvRect);
}
