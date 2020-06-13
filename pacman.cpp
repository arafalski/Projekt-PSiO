#include "pacman.hpp"

#include <cmath>

#include "consts.hpp"

Pacman::Pacman(sf::Texture* pacmanTexture, const sf::Vector2u& imageCount, float switchTime, float speed,
               sf::SoundBuffer& soundBuffer)
        : m_speed(speed), m_animation(pacmanTexture, imageCount, switchTime) {
    setSize(sf::Vector2f(40.0f, 40.0f));
    setOrigin(getSize() / 2.0f);
    setPosition(TILE, TILE);
    setTexture(pacmanTexture);

    m_hitSound.setBuffer(soundBuffer);
}

void Pacman::update(float deltaTime, const sf::Vector2f& targetPosition) {
    sf::Vector2f distance = targetPosition - getPosition();

    if (distance.x != 0 && distance.y != 0) {
        m_velocity.y = sqrtf(
                (powf(m_speed, 2.0f) * powf(distance.y, 2.0f)) / (powf(distance.x, 2.0f) + powf(distance.y, 2.0f)));
        m_velocity.x = (std::abs(distance.x) * m_velocity.y) / std::abs(distance.y);
    } else if (distance.x != 0) {
        m_velocity.y = 0;
        m_velocity.x = m_speed;
    } else if (distance.y != 0) {
        m_velocity.y = m_speed;
        m_velocity.x = 0;
    } else {
        m_velocity.y = 0;
        m_velocity.x = 0;
    }

    if (distance.x < 0) {
        m_velocity.x *= -1;
    }

    if (distance.y < 0) {
        m_velocity.y *= -1;
    }

    auto angle = static_cast<float>(atan2f(distance.x, -distance.y) * 180 / M_PI) - 90.0f;
    move(m_velocity * deltaTime);
    setRotation(angle);

    m_animation.update(deltaTime);
    setTextureRect(m_animation.uvRect);
}

void Pacman::onCollision(Direction dir) {
    if (dir == Direction::EAST || dir == Direction::WEST) {
        m_velocity.x = 0.0f;
    }

    if (dir == Direction::NORTH || dir == Direction::SOUTH) {
        m_velocity.y = 0.0f;
    }

    if (!duringCollision) {
        m_hitSound.play();
    }
}

Collider Pacman::getCollider() {
    return Collider(*this);
}
