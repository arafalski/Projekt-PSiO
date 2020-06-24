#ifndef PROJEKT_MAP_HPP
#define PROJEKT_MAP_HPP

#include <array>
#include <vector>

#include <SFML/Graphics.hpp>
#include "consts.hpp"
#include "light.hpp"
#include "pacman.hpp"
#include "tile.hpp"

class Map {
public:
    Map(sf::Texture& wallTexture, sf::Texture& startTexture, sf::Texture& endTexture, sf::Texture& pointTexture);

    void draw(sf::RenderWindow& window) const;

    void collisionDetection(Pacman& player, bool& endTileHit);

    Light& getLight() { return light; }

private:
    std::vector<Tile> m_mapGrid;

    struct Cell {
        std::array<std::array<char, 2>, 2> grid{{{'#', '#'},
                                                        {'#', ' '}}};
        bool visited = false;
    };

    Light light;

    void loadMapTolightSystem(const std::vector<std::vector<char>>& cells);

    std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT> generateTilesPlacement();

    std::vector<std::vector<char>> mazeToChar(const std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT>& maze);
};

#endif //PROJEKT_MAP_HPP
