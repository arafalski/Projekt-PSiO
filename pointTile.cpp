#include "pointTile.hpp"

PointTile::PointTile(sf::Texture& tileTexture, const sf::Vector2f& position)
        : Tile(tileTexture, position) {
    setSize(sf::Vector2f(TILE / 4.0f, TILE / 4.0f));
    setOrigin(TILE / 8.0f, TILE / 8.0f);
    m_tileSprite.setPosition(getPosition());
    m_tileSprite.setOrigin(getOrigin());
    m_tileSprite.setTextureRect(
            sf::IntRect(static_cast<int>(getPosition().x), static_cast<int>(getPosition().y),
                        static_cast<int>(TILE / 4),
                        static_cast<int>(TILE / 4)));
}
