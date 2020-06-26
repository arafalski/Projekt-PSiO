#ifndef PROJEKT_ENDTILE_HPP
#define PROJEKT_ENDTILE_HPP

#include "tile.hpp"

class EndTile : public Tile {
public:
    EndTile(sf::Texture& tileTexture, const sf::Vector2f& position);

    ~EndTile() override = default;

    bool isStartTile() const override { return false; }

    bool isEndTile() const override { return true; }

    bool isWallTile() const override { return false; }

    bool isPointTile() const override { return false; }
};


#endif //PROJEKT_ENDTILE_HPP
