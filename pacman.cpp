#include "pacman.hpp"

Pacman::Pacman(float speed) {
    this->speed = speed;
    body.setSize(sf::Vector2f(50.0f, 50.0f));
    body.setOrigin(body.getSize().x / 2, body.getSize().y / 2);
    body.setFillColor(sf::Color(255, 255, 0));
}

void Pacman::update(float deltaTime, sf::Vector2f targetPosition) {
    sf::Vector2f distance = targetPosition - body.getPosition();

    velocity.y = sqrtf(
            (speed * powf(distance.y, 2.0f)) / (powf(distance.x, 2.0f) + powf(distance.y, 2.0f)));
    velocity.x = (std::abs(distance.x) * velocity.y) / std::abs(distance.y);

    if(distance.x < 0){
        velocity.x *= -1;
    }

    if(distance.y < 0){
        velocity.y *= -1;
    }

    body.move(velocity * deltaTime);
}
