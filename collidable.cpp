#include "collidable.hpp"
#include <cmath>

bool Collidable::checkCollision(Collidable& other, Direction& dir) {
    sf::Vector2f delta = other.getPosition() - getPosition();
    sf::Vector2f minDistWithoutCollision = other.getSize() / 2.0f + getSize() / 2.0f;
    sf::Vector2f intersect(std::abs(delta.x) - minDistWithoutCollision.x,
                           std::abs(delta.y) - minDistWithoutCollision.y);

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
