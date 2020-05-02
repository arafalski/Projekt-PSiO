#ifndef PROJEKT_MAP_HPP
#define PROJEKT_MAP_HPP

#include <SFML/Graphics.hpp>
#include <array>
#include <vector>
#include <random>
#include <chrono>
#include <stack>
#include "consts.hpp"

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

std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT> generateTilesPlacement();

std::vector<std::vector<char>> mazeToChar(const std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT> &maze);

std::vector<sf::RectangleShape> generateMap();

#endif //PROJEKT_MAP_HPP
