#include "map.hpp"

#include <algorithm>
#include <chrono>
#include <map>
#include <mutex>
#include <random>
#include <stack>
#include <thread>

Map::Map(sf::Texture& wallTexture, sf::Texture& startTexture, sf::Texture& endTexture, sf::Texture& pointTexture) {
    std::vector<std::vector<char>> cells = mazeToChar(generateTilesPlacement());
    std::thread loadLight(&Map::loadMapTolightSystem, this, cells);

    for (std::size_t i = 0; i < cells.size(); i++) {
        for (std::size_t j = 0; j < cells[i].size(); j++) {

            sf::Vector2f position(static_cast<float>(j) * TILE, static_cast<float>(i) * TILE);
            switch (cells[i][j]) {
                case '#':
                    m_mapGrid.emplace_back(Tile(wallTexture, '#', position));
                    break;
                case 's':
                    m_mapGrid.emplace_back(Tile(startTexture, 's', position));
                    break;
                case 'e':
                    m_mapGrid.emplace_back(Tile(endTexture, 'e', position));
                    break;
                default:
                    m_mapGrid.emplace_back(Tile(pointTexture, 'p', position));
            }
        }
    }
    loadLight.join();
}

void Map::loadMapTolightSystem(const std::vector<std::vector<char>>& cells) {
    std::mutex m;
    const std::lock_guard<std::mutex> lock(m);

    light = Light(cells);
}

std::array<std::array<Map::Cell, MAP_WIDTH>, MAP_HEIGHT> Map::generateTilesPlacement() {
    std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT> maze{};
    std::stack<sf::Vector2u> backtrack;
    size_t visitedCells;

    std::random_device rd;
    std::mt19937 generator(rd());

    size_t longestPath = 0;
    sf::Vector2u endCell;

    //Adding starting point
    backtrack.push(sf::Vector2u(0, 0));
    visitedCells = 1;
    maze.front().front().visited = true;
    maze.front().front().grid[1][1] = 's';

    do {
        std::vector<Direction> neighbours;
        neighbours.reserve(4);

        if (backtrack.top().y > 0 && !maze[backtrack.top().y - 1][backtrack.top().x].visited) {
            neighbours.emplace_back(Direction::NORTH);
        }
        if (backtrack.top().y < MAP_HEIGHT - 1 && !maze[backtrack.top().y + 1][backtrack.top().x].visited) {
            neighbours.emplace_back(Direction::SOUTH);
        }
        if (backtrack.top().x < MAP_WIDTH - 1 && !maze[backtrack.top().y][backtrack.top().x + 1].visited) {
            neighbours.emplace_back(Direction::EAST);
        }
        if (backtrack.top().x > 0 && !maze[backtrack.top().y][backtrack.top().x - 1].visited) {
            neighbours.emplace_back(Direction::WEST);
        }
        neighbours.shrink_to_fit();

        if (!neighbours.empty()) {
            std::uniform_int_distribution<size_t> distrib(0, neighbours.size() - 1);
            Direction nextDir = neighbours[distrib(generator)];
            sf::Vector2u actualCell = backtrack.top();
            sf::Vector2u nextCell = backtrack.top();
            sf::Vector2u index;

            switch (nextDir) {
                case Direction::NORTH:
                    index.x = 1;
                    index.y = 0;
                    nextCell.y--;
                    break;
                case Direction::SOUTH:
                    index.x = 1;
                    index.y = 0;
                    actualCell.y++;
                    nextCell.y++;
                    break;
                case Direction::EAST:
                    index.x = 0;
                    index.y = 1;
                    actualCell.x++;
                    nextCell.x++;
                    break;
                case Direction::WEST:
                    index.x = 0;
                    index.y = 1;
                    nextCell.x--;
                    break;
            }

            maze[actualCell.y][actualCell.x].grid[index.y][index.x] = ' ';
            backtrack.push(nextCell);
            maze[backtrack.top().y][backtrack.top().x].visited = true;
            visitedCells++;
        } else {
            if (backtrack.size() >= longestPath) {
                longestPath = backtrack.size();
                endCell = backtrack.top();
            }
            backtrack.pop();
        }
    } while (visitedCells < MAP_WIDTH * MAP_HEIGHT);

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
        cell.drawSprite(window);
    }
}

void Map::collisionDetection(Pacman& player, bool& endTileHit) {
    Direction dir;

    auto hitPointIt = m_mapGrid.end();
    bool hitWall = false;
    for (std::size_t i = 0; i < m_mapGrid.size(); i++) {

        if (m_mapGrid[i].getFunction() == '#' && m_mapGrid[i].checkCollision(player, dir)) {
            player.onCollision(dir);
            player.duringCollision = true;
            hitWall = true;
        } else if (m_mapGrid[i].getFunction() == 'e' &&
                   m_mapGrid[i].getGlobalBounds().intersects(player.getGlobalBounds())) {
            endTileHit = true;
        } else if (m_mapGrid[i].getFunction() == 'p' &&
                   m_mapGrid[i].getGlobalBounds().intersects(player.getGlobalBounds())) {
            hitPointIt = std::next(m_mapGrid.begin(), static_cast<int>(i));
        }
    }

    if (!hitWall) {
        player.duringCollision = false;
    }

    if (hitPointIt != m_mapGrid.end()) {
        m_mapGrid.erase(hitPointIt);
    }
}
