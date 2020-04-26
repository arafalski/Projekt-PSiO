#include "character.hpp"

Character::Character(sf::Texture *texture, sf::Vector2u imageCount, float switchTime, float speed)
        : speed(speed), animation(texture, imageCount, switchTime) {}
