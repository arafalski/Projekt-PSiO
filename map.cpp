#include "map.hpp"

std::vector<sf::RectangleShape> generateMap() {
    std::vector<sf::RectangleShape> obstacles;

    std::array<std::array<short, MAP_WIDTH>, MAP_HEIGHT> tiles = generateTilesPlacement();

    for (unsigned int i = 0; i < MAP_HEIGHT; i++) {
        for (unsigned int j = 0; j < MAP_WIDTH; j++) {
            if (tiles[i][j] == 1) {
                sf::RectangleShape box;
                box.setSize(sf::Vector2f(TILE, TILE));
                box.setPosition(static_cast<float>(j) * TILE, static_cast<float>(i) * TILE);
                box.setFillColor(sf::Color::Blue);

                obstacles.emplace_back(box);
            }
        }
    }

    return obstacles;
}

std::array<std::array<short, MAP_WIDTH>, MAP_HEIGHT> generateTilesPlacement() {
    std::array<std::array<short, MAP_WIDTH>, MAP_HEIGHT> tiles{};
    std::mt19937 generator(
            static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));

    for (unsigned int i = 0; i < MAP_HEIGHT; i++) {
        for (unsigned int j = 0; j < MAP_WIDTH; j++) {
            tiles[i][j] = static_cast<short>(generator() % 2);
        }
    }

    return tiles;
}
