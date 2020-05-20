#include "tile.hpp"

Tile::Tile(sf::Texture &tileTexture, const char &function, const sf::Vector2f &position) : m_function(function) {
    setTexture(&tileTexture);

    setPosition(position);

    if (function != 'p') {
        setSize(sf::Vector2f(TILE, TILE));
        setOrigin(TILE / 2.0f, TILE / 2.0f);
    } else {
        setSize(sf::Vector2f(TILE / 4.0f, TILE / 4.0f));
        setOrigin(TILE / 8.0f, TILE / 8.0f);
    }
}

Collider Tile::getCollider() {
    return Collider(*this);
}

char Tile::getFunction() const {
    return m_function;
}
