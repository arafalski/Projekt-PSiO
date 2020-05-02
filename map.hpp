#ifndef PROJEKT_MAP_HPP
#define PROJEKT_MAP_HPP

#include <SFML/Graphics.hpp>
#include <array>
#include <vector>
#include <random>
#include <chrono>
#include "consts.hpp"

enum class Directions {
    UP,
    DOWN,
    LEFT,
    RIGHT,
};

std::array<std::array<char, MAP_WIDTH>, MAP_HEIGHT> generateTilesPlacement();

std::vector<sf::RectangleShape> generateMap();

#endif //PROJEKT_MAP_HPP
