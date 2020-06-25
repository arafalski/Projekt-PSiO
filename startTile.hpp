#ifndef PROJEKT_STARTTILE_HPP
#define PROJEKT_STARTTILE_HPP

#include "tile.hpp"

class StartTile : public Tile {
public:
    StartTile(sf::Texture& tileTexture, const sf::Vector2f& position);

    ~StartTile() override = default;

    bool isStartTile() const override { return true; }

    bool isEndTile() const override { return false; }

    bool isWallTile() const override { return false; }

    bool isPointTile() const override { return false; }
};


#endif //PROJEKT_STARTTILE_HPP
