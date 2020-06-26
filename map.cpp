#include "map.hpp"

#include <algorithm>
#include <chrono>
#include <map>
#include <mutex>
#include <random>
#include <stack>
#include <thread>

#include "endTile.hpp"
#include "pointTile.hpp"
#include "startTile.hpp"
#include "wallTile.hpp"

Map::Map(sf::Texture& wallTexture, sf::Texture& startTexture, sf::Texture& endTexture, sf::Texture& pointTexture) {
    std::vector<std::vector<char>> cells = mazeToChar(generateTilesPlacement());
    std::thread loadLight(&Map::loadMapToLightSystem, this, cells);

    for (std::size_t i = 0; i < cells.size(); i++) {
        for (std::size_t j = 0; j < cells[i].size(); j++) {

            sf::Vector2f position(static_cast<float>(j) * TILE, static_cast<float>(i) * TILE);
            switch (cells[i][j]) {
                case '#':
                    m_mapGrid.push_back(std::make_unique<WallTile>(WallTile(wallTexture, position)));
                    break;
                case 's':
                    m_mapGrid.push_back(std::make_unique<StartTile>(StartTile(startTexture, position)));
                    break;
                case 'e':
                    m_mapGrid.push_back(std::make_unique<EndTile>(EndTile(endTexture, position)));
                    break;
                default:
                    m_mapGrid.push_back(std::make_unique<PointTile>(PointTile(pointTexture, position)));
            }
        }
    }
    loadLight.join();
}

void Map::loadMapToLightSystem(const std::vector<std::vector<char>>& cells) {
    std::mutex m;
    const std::lock_guard<std::mutex> lock(m);

    light = Light(cells);
}

std::array<std::array<Map::Cell, MAP_WIDTH>, MAP_HEIGHT> Map::generateTilesPlacement() {
    std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT> maze{};
    std::stack<sf::Vector2u> backtrack;
    size_t visitedCells;

    std::mt19937 generator(
            static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));

    size_t longestPath = 0;
    sf::Vector2u endCell;

    //Adding starting point
    backtrack.emplace(sf::Vector2u(0, 0));
    visitedCells = 1;
    maze.front().front().visited = true;
    maze.front().front().grid[1][1] = 's';

    while (!backtrack.empty()) {
        sf::Vector2u currentCell = backtrack.top();
        if (backtrack.size() >= longestPath) {
            longestPath = backtrack.size();
            endCell = currentCell;
        }
        backtrack.pop();

        std::vector<Direction> neighbours;
        neighbours.reserve(4);

        if (currentCell.y > 0 && !maze[currentCell.y - 1][currentCell.x].visited) {
            neighbours.emplace_back(Direction::NORTH);
        }
        if (currentCell.y < MAP_HEIGHT - 1 && !maze[currentCell.y + 1][currentCell.x].visited) {
            neighbours.emplace_back(Direction::SOUTH);
        }
        if (currentCell.x < MAP_WIDTH - 1 && !maze[currentCell.y][currentCell.x + 1].visited) {
            neighbours.emplace_back(Direction::EAST);
        }
        if (currentCell.x > 0 && !maze[currentCell.y][currentCell.x - 1].visited) {
            neighbours.emplace_back(Direction::WEST);
        }
        neighbours.shrink_to_fit();

        if (!neighbours.empty()) {
            backtrack.push(currentCell);
            Direction nextDir = neighbours[generator() % neighbours.size()];
            sf::Vector2u chosenCell = currentCell;

            switch (nextDir) {
                case Direction::NORTH:
                    chosenCell.y--;
                    maze[currentCell.y][currentCell.x].grid[0][1] = ' ';
                    break;
                case Direction::SOUTH:
                    chosenCell.y++;
                    maze[currentCell.y + 1][currentCell.x].grid[0][1] = ' ';
                    break;
                case Direction::EAST:
                    chosenCell.x++;
                    maze[currentCell.y][currentCell.x + 1].grid[1][0] = ' ';
                    break;
                case Direction::WEST:
                    chosenCell.x--;
                    maze[currentCell.y][currentCell.x].grid[1][0] = ' ';
                    break;
            }

            backtrack.push(chosenCell);
            maze[chosenCell.y][chosenCell.x].visited = true;
            visitedCells++;
        }
    }
    maze[endCell.y][endCell.x].grid[1][1] = 'e';

    return maze;
}

std::vector<std::vector<char>> Map::mazeToChar(const std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT>& maze) {
    std::vector<std::vector<char>> cells;
    cells.reserve(MAP_HEIGHT * 2 + 1);

    for (auto& row : maze) {
        std::vector<char> temp;
        temp.reserve(MAP_WIDTH * 2 + 1);

        for (auto& cell : row) {
            temp.emplace_back(cell.grid[0][0]);
            temp.emplace_back(cell.grid[0][1]);
        }
        temp.emplace_back('#');

        std::vector<char> temp2;
        temp2.reserve(MAP_WIDTH * 2 + 1);

        for (auto& cell : row) {
            temp2.emplace_back(cell.grid[1][0]);
            temp2.emplace_back(cell.grid[1][1]);
        }
        temp2.emplace_back('#');

        cells.emplace_back(temp);
        cells.emplace_back(temp2);
    }
    std::vector<char> endingRow(MAP_WIDTH * 2 + 1, '#');
    cells.emplace_back(endingRow);

    return cells;
}

void Map::draw(sf::RenderWindow& window) const {
    for (const auto& cell : m_mapGrid) {
        cell->drawSprite(window);
    }
}

void Map::collisionDetection(Pacman& player, bool& endTileHit) {
    Direction dir;

    auto hitPointIt = m_mapGrid.end();
    bool hitWall = false;

    for (auto it = m_mapGrid.begin(); it != m_mapGrid.end(); ++it) {
        if ((*it)->isWallTile() && player.checkCollision(**it, dir)) {
            player.onCollision(dir);
            player.duringCollision = true;
            hitWall = true;
        } else if ((*it)->isEndTile() && (*it)->getGlobalBounds().intersects(player.getGlobalBounds())) {
            endTileHit = true;
        } else if ((*it)->isPointTile() && (*it)->getGlobalBounds().intersects(player.getGlobalBounds())) {
            hitPointIt = it;
        }
    }

    if (!hitWall) {
        player.duringCollision = false;
    }

    if (hitPointIt != m_mapGrid.end()) {
        m_mapGrid.erase(hitPointIt);
    }
}
