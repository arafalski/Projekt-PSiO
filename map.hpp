#ifndef PROJEKT_MAP_HPP
#define PROJEKT_MAP_HPP

#include <SFML/Graphics.hpp>
#include <array>
#include <vector>
#include <random>
#include <chrono>
#include <stack>
#include <map>
#include <thread>
#include <mutex>
#include <algorithm>
#include "cell.hpp"
#include "consts.hpp"
#include "pacman.hpp"

class Map {
    enum class Direction {
        NORTH,
        SOUTH,
        EAST,
        WEST
    };

    struct Edge {
        float start_x, start_y;
        float end_x, end_y;
    };

    struct polyCell {
        std::map<Direction, std::pair<std::size_t, bool>> edge{{Direction::NORTH, std::make_pair(0, false)},
                                                       {Direction::SOUTH, std::make_pair(0, false)},
                                                       {Direction::EAST,  std::make_pair(0, false)},
                                                       {Direction::WEST,  std::make_pair(0, false)}};
        bool exist = false;
    };

    std::vector<sf::RectangleShape> mapGrid;

    std::vector<Edge> edges;

    std::vector<std::pair<float, sf::Vector2f>> visiblePolyPoints; //angle, x, y

    void generate();

    std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT> generateTilesPlacement();

    std::vector<std::vector<char>> mazeToChar(const std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT> &maze);

    void convertTileMapToPolyMap(const std::vector<std::vector<char>> &cells);

public:
    Map();

    ~Map() = default;

    void draw(sf::RenderWindow &window) const;

    void collisionDetection(Pacman &player, bool &endTileHit);

    void checkVisibility(const sf::Vector2f &playerPos, const float &radius);

    void drawLight(const sf::Vector2f &playerPos, sf::RenderWindow &window);
};

#endif //PROJEKT_MAP_HPP
