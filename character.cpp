#include "character.hpp"

sf::Vector2f Character::getPosition() {
    return sf::Vector2f(body.getPosition().x, body.getPosition().y);
}

void Character::draw(sf::RenderWindow &window) {
    window.draw(body);
}
