#include "animation.hpp"

Animation::Animation(sf::Texture *texture, sf::Vector2u imageCount, float switchTime)
        : imageCount(imageCount), totalTime(0.0f), switchTime(switchTime) {
    currentImage.x = 0;
    uvRect.width = static_cast<int>(texture->getSize().x / imageCount.x);
    uvRect.height = static_cast<int>(texture->getSize().y / imageCount.y);
}

void Animation::update(float deltaTime, unsigned int row) {
    currentImage.y = row;
    totalTime += deltaTime;

    if (totalTime >= switchTime) {
        totalTime -= switchTime;
        currentImage.x++;

        if (currentImage.x >= imageCount.x) {
            currentImage.x = 0;
        }
    }

    uvRect.top = static_cast<int>(currentImage.y * static_cast<unsigned int>(uvRect.height));
    uvRect.left = static_cast<int>(currentImage.x * static_cast<unsigned int>(uvRect.width));
}
