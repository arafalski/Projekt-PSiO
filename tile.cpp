#include "tile.hpp"

#include "consts.hpp"

Tile::Tile(sf::Texture& tileTexture, const sf::Vector2f& position) {
    setPosition(position);
    m_tileSprite.setTexture(tileTexture);
}

void Tile::drawSprite(sf::RenderWindow& window) const {
    window.draw(m_tileSprite);
}
