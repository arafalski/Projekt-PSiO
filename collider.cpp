#include "collider.hpp"
#include <cmath>

Collider::Collider(sf::RectangleShape& body) : m_body(body) {}

void Collider::move(float dx, float dy) {
    m_body.move(dx, dy);
}

bool Collider::checkCollision(Collider& other, Direction& dir) {
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

sf::Vector2f Collider::getPosition() const {
    return m_body.getPosition();
}

sf::Vector2f Collider::getHalfSize() const {
    return m_body.getSize() / 2.0f;
}
