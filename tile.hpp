#ifndef PROJEKT_TILE_HPP
#define PROJEKT_TILE_HPP

#include <SFML/Graphics.hpp>
#include "collider.hpp"
#include "consts.hpp"

class Tile : public sf::RectangleShape {
public:
    Tile(sf::Texture &tileTexture, const char &function, const sf::Vector2f &position);

    Collider getCollider();

    char getFunction() const;

private:
    char m_function;
};


#endif //PROJEKT_TILE_HPP