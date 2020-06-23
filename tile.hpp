#ifndef PROJEKT_TILE_HPP
#define PROJEKT_TILE_HPP

#include <SFML/Graphics.hpp>
#include "collidable.hpp"

class Tile : public Collidable {
public:
    Tile(sf::Texture& tileTexture, char function, const sf::Vector2f& position);

    char getFunction() const;

    void drawSprite(sf::RenderWindow& window) const;

private:
    char m_function;

    sf::Sprite m_tileSprite;
};


#endif //PROJEKT_TILE_HPP
