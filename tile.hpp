#ifndef PROJEKT_TILE_HPP
#define PROJEKT_TILE_HPP

#include <SFML/Graphics.hpp>
#include "collidable.hpp"

class Tile : public Collidable {
public:
    Tile(sf::Texture& tileTexture, const sf::Vector2f& position);

    ~Tile() override = default;

    void drawSprite(sf::RenderWindow& window) const;

    virtual bool isStartTile() const = 0;

    virtual bool isEndTile() const = 0;

    virtual bool isWallTile() const = 0;

    virtual bool isPointTile() const = 0;

protected:
    sf::Sprite m_tileSprite;
};


#endif //PROJEKT_TILE_HPP
