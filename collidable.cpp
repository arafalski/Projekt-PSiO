#include "collidable.hpp"
#include <cmath>

bool Collidable::checkCollision(Collidable& other, Direction& dir) {
    sf::Vector2f betweenCenters = getPosition() - other.getPosition();
    sf::Vector2f minDistWithoutCollision = getSize() / 2.0f + other.getSize() / 2.0f;
    sf::Vector2f intersectionDist(std::abs(betweenCenters.x) - minDistWithoutCollision.x,
                                  std::abs(betweenCenters.y) - minDistWithoutCollision.y);

    if (intersectionDist.x < 0.0f && intersectionDist.y < 0.0f) {
        if (std::abs(intersectionDist.x) <= std::abs(intersectionDist.y)) {
            if (betweenCenters.x > 0.0f) {
                move(-intersectionDist.x, 0.0f);
                dir = Direction::EAST;
            } else {
                move(intersectionDist.x, 0.0f);
                dir = Direction::WEST;
            }
        } else {
            if (betweenCenters.y > 0.0f) {
                move(0.0f, -intersectionDist.y);
                dir = Direction::SOUTH;
            } else {
                move(0.0f, intersectionDist.y);
                dir = Direction::NORTH;
            }
        }
        return true;
    }
    return false;
}
