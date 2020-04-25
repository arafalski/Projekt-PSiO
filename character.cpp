#include "character.hpp"

sf::Vector2f Character::getPosition() const {
    return body.getPosition();
}

void Character::draw(sf::RenderWindow &window) const {
    window.draw(body);
}
