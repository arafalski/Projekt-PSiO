#ifndef PROJEKT_COLLIDER_HPP
#define PROJEKT_COLLIDER_HPP

#include <SFML/Graphics.hpp>
#include <cmath>

class Collider {
public:
    explicit Collider(sf::RectangleShape &body);

    ~Collider() = default;

    void move(const float &dx, const float &dy);

    bool checkCollision(Collider &other, sf::Vector2f &direction);

    sf::Vector2f getPosition() const;

    sf::Vector2f getHalfSize() const;

private:
    sf::RectangleShape &m_body;
};

#endif //PROJEKT_COLLIDER_HPP
