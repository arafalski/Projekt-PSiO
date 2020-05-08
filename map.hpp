#ifndef PROJEKT_MAP_HPP
#define PROJEKT_MAP_HPP

#include <SFML/Graphics.hpp>
#include <array>
#include <vector>
#include <random>
#include <chrono>
#include <stack>
#include "consts.hpp"
#include "pacman.hpp"

enum class Direction {
    NORTH,
    SOUTH,
    EAST,
    WEST
};

class Cell {
    bool visited = false;

public:
    std::array<std::array<char, 2>, 2> grid{{{'#', '#'}, {'#', ' '}}};

    Cell() = default;

    ~Cell() = default;

    void visit() {
        visited = true;
    }

    bool wasVisited() const {
        return visited;
    }
};

class Map{
    std::vector<sf::RectangleShape> mapGrid;

    void generate();

    std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT> generateTilesPlacement();

    std::vector<std::vector<char>> mazeToChar(const std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT> &maze);

public:
    Map();

    ~Map() = default;

    void draw(sf::RenderWindow &window) const;

    void collisionDetection(Pacman &player, bool &endTileHit);
};

#endif //PROJEKT_MAP_HPP
