#ifndef PROJEKT_COLLIDER_HPP
#define PROJEKT_COLLIDER_HPP

#include <SFML/Graphics.hpp>

class Collider {
public:
    explicit Collider(sf::RectangleShape& body);

    ~Collider() = default;

    void move(float dx, float dy);

    bool checkCollision(Collider& other, sf::Vector2f& direction);

    sf::Vector2f getPosition() const;

    sf::Vector2f getHalfSize() const;

private:
    sf::RectangleShape& m_body;
};

#endif //PROJEKT_COLLIDER_HPP
