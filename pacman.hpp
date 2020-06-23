#ifndef PROJEKT_PACMAN_HPP
#define PROJEKT_PACMAN_HPP

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "collidable.hpp"
#include "consts.hpp"

class Pacman : public Collidable {
public:
    Pacman(sf::Texture* pacmanTexture, size_t imageCount, float switchTime, float speed, sf::SoundBuffer& soundBuffer);

    ~Pacman() override = default;

    void update(float deltaTime, const sf::Vector2f& targetPosition);

    void animationUpdate(float deltaTime);

    void onCollision(Direction dir);

    bool duringCollision = false;

private:
    float m_speed;

    sf::Vector2f m_velocity;

    sf::Sound m_hitSound;

    size_t m_imageCount;

    size_t m_currentImage = 0;

    float m_totalTime;

    float m_switchTime;

    sf::IntRect m_textureRect;
};


#endif //PROJEKT_PACMAN_HPP
