#ifndef PROJEKT_POINTTILE_HPP
#define PROJEKT_POINTTILE_HPP

#include "tile.hpp"

class PointTile : public Tile {
public:
    PointTile(sf::Texture& tileTexture, const sf::Vector2f& position);

    ~PointTile() override = default;

    bool isStartTile() const override { return false; }

    bool isEndTile() const override { return false; }

    bool isWallTile() const override { return false; }

    bool isPointTile() const override { return true; }
};


#endif //PROJEKT_POINTTILE_HPP
