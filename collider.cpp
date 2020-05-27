#include "collider.hpp"
#include <cmath>

Collider::Collider(sf::RectangleShape &body) : m_body(body) {}

void Collider::move(const float &dx, const float &dy) {
    m_body.move(dx, dy);
}

bool Collider::checkCollision(Collider &other, sf::Vector2f &direction) {
    sf::Vector2f otherPosition = other.getPosition();
    sf::Vector2f otherHalfSize = other.getHalfSize();
    sf::Vector2f thisPosition = getPosition();
    sf::Vector2f thisHalfSize = getHalfSize();

    float deltaX = otherPosition.x - thisPosition.x;
    float deltaY = otherPosition.y - thisPosition.y;

    float intersectX = std::abs(deltaX) - (otherHalfSize.x + thisHalfSize.x);
    float intersectY = std::abs(deltaY) - (otherHalfSize.y + thisHalfSize.y);

    if (intersectX < 0.0f && intersectY < 0.0f) {
        if (intersectX > intersectY) {
            if (deltaX > 0.0f) {
                move(0.0f, 0.0f);
                other.move(-intersectX, 0.0f);

                direction.x = 1.0f;
                direction.y = 0.0f;
            } else {
                move(0.0f, 0.0f);
                other.move(intersectX, 0.0f);

                direction.x = -1.0f;
                direction.y = 0.0f;
            }
        } else {
            if (deltaY > 0.0f) {
                move(0.0f, 0.0f);
                other.move(0.0f, -intersectY);

                direction.x = 0.0f;
                direction.y = 1.0f;
            } else {
                move(0.0f, 0.0f);
                other.move(0.0f, intersectY);

                direction.x = 0.0f;
                direction.y = -1.0f;
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
