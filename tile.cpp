#include "tile.hpp"
#include "consts.hpp"

Tile::Tile(sf::Texture &tileTexture, const char &function, const sf::Vector2f &position) : m_function(function) {
    setPosition(position);

    m_tileSprite.setTexture(tileTexture);

    if (function != 'p') {
        setSize(sf::Vector2f(TILE, TILE));
        setOrigin(TILE / 2.0f, TILE / 2.0f);
        m_tileSprite.setPosition(getPosition());
        m_tileSprite.setOrigin(getOrigin());
        m_tileSprite.setTextureRect(
                sf::IntRect(static_cast<int>(getPosition().x), static_cast<int>(getPosition().y),
                            static_cast<int>(TILE),
                            static_cast<int>(TILE)));
    } else {
        setSize(sf::Vector2f(TILE / 4.0f, TILE / 4.0f));
        setOrigin(TILE / 8.0f, TILE / 8.0f);
        m_tileSprite.setPosition(getPosition());
        m_tileSprite.setOrigin(getOrigin());
        m_tileSprite.setTextureRect(
                sf::IntRect(static_cast<int>(getPosition().x), static_cast<int>(getPosition().y),
                            static_cast<int>(TILE / 4),
                            static_cast<int>(TILE / 4)));
    }
}

Collider Tile::getCollider() {
    return Collider(*this);
}

char Tile::getFunction() const {
    return m_function;
}

void Tile::drawSprite(sf::RenderWindow &window) const {
    window.draw(m_tileSprite);
}
