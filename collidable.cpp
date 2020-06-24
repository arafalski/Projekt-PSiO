#include "collidable.hpp"
#include <cmath>

bool Collidable::checkCollision(Collidable& other, Direction& dir) {
    sf::Vector2f otherHalfSize = other.getHalfSize();
    sf::Vector2f thisHalfSize = getHalfSize();

    sf::Vector2f delta = other.getPosition() - getPosition();

    sf::Vector2f intersect;
    intersect.x = std::abs(delta.x) - (otherHalfSize.x + thisHalfSize.x);
    intersect.y = std::abs(delta.y) - (otherHalfSize.y + thisHalfSize.y);

    if (intersect.x < 0.0f && intersect.y < 0.0f) {
        if (intersect.x > intersect.y) {
            if (delta.x > 0.0f) {
                other.move(-intersect.x, 0.0f);
                dir = Direction::EAST;
            } else {
                other.move(intersect.x, 0.0f);
                dir = Direction::WEST;
            }
        } else {
            if (delta.y > 0.0f) {
                other.move(0.0f, -intersect.y);
                dir = Direction::SOUTH;
            } else {
                other.move(0.0f, intersect.y);
                dir = Direction::NORTH;
            }
        }

        return true;
    }

    return false;
}

sf::Vector2f Collidable::getHalfSize() const {
    return getSize() / 2.0f;
}
