#ifndef PROJEKT_PACMAN_HPP
#define PROJEKT_PACMAN_HPP

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "animation.hpp"
#include "collider.hpp"

class Pacman : public sf::RectangleShape {
public:
    Pacman(sf::Texture *pacmanTexture, const sf::Vector2u &imageCount, float switchTime, float speed, sf::SoundBuffer &soundBuffer);

    ~Pacman() override = default;

    void update(float deltaTime, const sf::Vector2f &targetPosition);

    void onCollision(const sf::Vector2f &direction);

    Collider getCollider();

    bool duringCollision = false;

private:
    float m_speed;

    sf::Vector2f m_velocity;

    Animation m_animation;

    sf::Sound m_hitSound;
};


#endif //PROJEKT_PACMAN_HPP
