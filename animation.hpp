#ifndef PROJEKT_ANIMATION_HPP
#define PROJEKT_ANIMATION_HPP

#include <SFML/Graphics.hpp>

class Animation {
    sf::Vector2u imageCount;
    sf::Vector2u currentImage;

    float totalTime;
    float switchTime;
public:
    sf::IntRect uvRect;

    Animation(sf::Texture *texture, sf::Vector2u imageCount, float switchTime);

    void update(float deltaTime, unsigned int row = 0);
};


#endif //PROJEKT_ANIMATION_HPP
