#include "pacman.hpp"

#include <cmath>

#include "consts.hpp"

Pacman::Pacman(sf::Texture* pacmanTexture, size_t imageCount, float switchTime, float speed,
               sf::SoundBuffer& soundBuffer)
        : m_speed(speed), m_imageCount(imageCount), m_switchTime(switchTime) {
    setSize(sf::Vector2f(40.0f, 40.0f));
    setOrigin(getSize() / 2.0f);
    setPosition(TILE, TILE);
    setTexture(pacmanTexture);

    m_textureRect.top = 0;
    m_textureRect.width = static_cast<int>(pacmanTexture->getSize().x / imageCount);
    m_textureRect.height = static_cast<int>(pacmanTexture->getSize().y);

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

    animationUpdate(deltaTime);
    setTextureRect(m_textureRect);
}

void Pacman::animationUpdate(float deltaTime) {
    m_totalTime += deltaTime;

    if (m_totalTime >= m_switchTime) {
        m_totalTime -= m_switchTime;
        m_currentImage++;

        if (m_currentImage >= m_imageCount) {
            m_currentImage = 0;
        }
    }

    m_textureRect.left = static_cast<int>(m_currentImage * static_cast<size_t>(m_textureRect.width));
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
