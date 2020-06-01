#ifndef PROJEKT_MAP_HPP
#define PROJEKT_MAP_HPP

#include <array>
#include <vector>

#include <SFML/Graphics.hpp>
#include "cell.hpp"
#include "consts.hpp"
#include "pacman.hpp"
#include "tile.hpp"

class Map {
public:
    Map(sf::Texture &wallTexture, sf::Texture &startTexture, sf::Texture &endTexture, sf::Texture &pointTexture);

    ~Map() = default;

    void draw(sf::RenderWindow &window) const;

    void collisionDetection(Pacman &player, bool &endTileHit);

    void checkVisibility(const Pacman &player);

    void drawLight(const sf::Vector2f &playerPos, sf::RenderWindow &window);

private:
    enum class Direction {
        NORTH,
        SOUTH,
        EAST,
        WEST
    };

    struct Edge {
        sf::Vector2f start;
        sf::Vector2f end;
    };

    struct polyCell {
        std::map<Direction, std::pair<std::size_t, bool>> edge{{Direction::NORTH, std::make_pair(0, false)},
                                                               {Direction::SOUTH, std::make_pair(0, false)},
                                                               {Direction::EAST,  std::make_pair(0, false)},
                                                               {Direction::WEST,  std::make_pair(0, false)}};
        bool exist = false;
    };

    std::vector<Tile> m_mapGrid;

    std::vector<Edge> m_edges;

    std::vector<std::pair<float, sf::Vector2f>> m_visiblePolyPoints; //angle, x, y

    std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT> generateTilesPlacement();

    std::vector<std::vector<char>> mazeToChar(const std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT> &maze);

    void convertTileMapToPolyMap(const std::vector<std::vector<char>> &cells);

    void addPolyMapBoundary();

    void sortAndEraseDuplicatesVisiblePoints();

    void checkIntersection(float angle, const sf::Vector2f &playerPos);
};

#endif //PROJEKT_MAP_HPP
