#ifndef PROJEKT_WALLTILE_HPP
#define PROJEKT_WALLTILE_HPP

#include "tile.hpp"

class WallTile : public Tile {
public:
    WallTile(sf::Texture& tileTexture, const sf::Vector2f& position);

    ~WallTile() override = default;

    bool isStartTile() const override { return false; }

    bool isEndTile() const override { return false; }

    bool isWallTile() const override { return true; }

    bool isPointTile() const override { return false; }
};


#endif //PROJEKT_WALLTILE_HPP
