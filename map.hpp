#ifndef PROJEKT_MAP_HPP
#define PROJEKT_MAP_HPP

#include <SFML/Graphics.hpp>
#include <array>
#include <vector>
#include <random>
#include <chrono>
#include "consts.hpp"

std::array<std::array<short, MAP_WIDTH>, MAP_HEIGHT> generateTilesPlacement();

std::vector<sf::RectangleShape> generateMap();

#endif //PROJEKT_MAP_HPP
