#ifndef PROJEKT_COLLIDER_HPP
#define PROJEKT_COLLIDER_HPP

#include <SFML/Graphics.hpp>
#include <cmath>

class Collider {
    sf::RectangleShape &body;

public:
    explicit Collider(sf::RectangleShape &body);

    ~Collider() = default;

    void Move(const float &dx, const float &dy);

    bool checkCollision(Collider &other, sf::Vector2f &direction);

    sf::Vector2f getPosition() const;

    sf::Vector2f getHalfSize() const;
};

#endif //PROJEKT_COLLIDER_HPP
