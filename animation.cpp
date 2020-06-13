#include "animation.hpp"

Animation::Animation(sf::Texture* texture, sf::Vector2u imageCount, float switchTime)
        : m_imageCount(imageCount), m_totalTime(0.0f), m_switchTime(switchTime) {
    m_currentImage.x = 0;
    uvRect.width = static_cast<int>(texture->getSize().x / imageCount.x);
    uvRect.height = static_cast<int>(texture->getSize().y / imageCount.y);
}

void Animation::update(float deltaTime, unsigned int row) {
    m_currentImage.y = row;
    m_totalTime += deltaTime;

    if (m_totalTime >= m_switchTime) {
        m_totalTime -= m_switchTime;
        m_currentImage.x++;

        if (m_currentImage.x >= m_imageCount.x) {
            m_currentImage.x = 0;
        }
    }

    uvRect.top = static_cast<int>(m_currentImage.y * static_cast<unsigned int>(uvRect.height));
    uvRect.left = static_cast<int>(m_currentImage.x * static_cast<unsigned int>(uvRect.width));
}
