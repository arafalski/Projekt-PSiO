#include "wallTile.hpp"

WallTile::WallTile(sf::Texture& tileTexture, const sf::Vector2f& position)
        : Tile(tileTexture, position) {
    setSize(sf::Vector2f(TILE, TILE));
    setOrigin(TILE / 2.0f, TILE / 2.0f);
    m_tileSprite.setPosition(getPosition());
    m_tileSprite.setOrigin(getOrigin());
    m_tileSprite.setTextureRect(
            sf::IntRect(static_cast<int>(getPosition().x), static_cast<int>(getPosition().y),
                        static_cast<int>(TILE),
                        static_cast<int>(TILE)));
}
