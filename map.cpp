#include "map.hpp"

std::vector<sf::RectangleShape> generateMap() {
    std::vector<sf::RectangleShape> obstacles;

    std::vector<std::vector<char>> cells = mazeToChar(generateTilesPlacement());

    for (std::size_t i = 0; i < cells.size(); i++) {
        for (std::size_t j = 0; j < cells[i].size(); j++) {
            sf::RectangleShape box;
            box.setSize(sf::Vector2f(TILE, TILE));
            box.setPosition(static_cast<float>(j) * TILE, static_cast<float>(i) * TILE);
            box.setOrigin(TILE / 2.0f, TILE / 2.0f);
            switch (cells[i][j]) {
                case '#':
                    box.setFillColor(sf::Color::Blue);
                    obstacles.emplace_back(box);
                    break;
                case 's':
                    box.setFillColor(sf::Color::Green);
                    obstacles.emplace_back(box);
                    break;
                case 'e':
                    box.setFillColor(sf::Color::Red);
                    obstacles.emplace_back(box);
                    break;
                default:
                    break;
            }
        }
    }

    return obstacles;
}

std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT> generateTilesPlacement() {
    std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT> maze{};
    std::stack<std::pair<unsigned int, unsigned int>> backtrack;
    unsigned int visitedCells;
    std::mt19937 generator(
            static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));

    std::size_t longestPath = 0;
    std::pair<unsigned int, unsigned int> endCell;

    backtrack.push(std::make_pair(0, 0));
    visitedCells = 1;
    maze[0][0].visit();
    maze[0][0].grid[1][1] = 's';

    do {
        std::vector<Direction> neighbours;

        if (backtrack.top().second > 0 && !maze[backtrack.top().second - 1][backtrack.top().first].wasVisited()) {
            neighbours.emplace_back(Direction::NORTH);
        }
        if (backtrack.top().second < MAP_HEIGHT - 1 &&
            !maze[backtrack.top().second + 1][backtrack.top().first].wasVisited()) {
            neighbours.emplace_back(Direction::SOUTH);
        }
        if (backtrack.top().first < MAP_WIDTH - 1 &&
            !maze[backtrack.top().second][backtrack.top().first + 1].wasVisited()) {
            neighbours.emplace_back(Direction::EAST);
        }
        if (backtrack.top().first > 0 && !maze[backtrack.top().second][backtrack.top().first - 1].wasVisited()) {
            neighbours.emplace_back(Direction::WEST);
        }

        if (!neighbours.empty()) {
            Direction nextDir = neighbours[generator() % neighbours.size()];

            switch (nextDir) {
                case Direction::NORTH:
                    maze[backtrack.top().second][backtrack.top().first].grid[0][1] = ' ';
                    backtrack.push(std::make_pair(backtrack.top().first, backtrack.top().second - 1));
                    maze[backtrack.top().second][backtrack.top().first].visit();
                    visitedCells++;
                    break;
                case Direction::SOUTH:
                    maze[backtrack.top().second + 1][backtrack.top().first].grid[0][1] = ' ';
                    backtrack.push(std::make_pair(backtrack.top().first, backtrack.top().second + 1));
                    maze[backtrack.top().second][backtrack.top().first].visit();
                    visitedCells++;
                    break;
                case Direction::EAST:
                    maze[backtrack.top().second][backtrack.top().first + 1].grid[1][0] = ' ';
                    backtrack.push(std::make_pair(backtrack.top().first + 1, backtrack.top().second));
                    maze[backtrack.top().second][backtrack.top().first].visit();
                    visitedCells++;
                    break;
                case Direction::WEST:
                    maze[backtrack.top().second][backtrack.top().first].grid[1][0] = ' ';
                    backtrack.push(std::make_pair(backtrack.top().first - 1, backtrack.top().second));
                    maze[backtrack.top().second][backtrack.top().first].visit();
                    visitedCells++;
                    break;
                default:
                    break;
            }
        } else {
            if (backtrack.size() >= longestPath) {
                longestPath = backtrack.size();
                endCell = backtrack.top();
            }
            backtrack.pop();
        }
    } while (visitedCells < MAP_WIDTH * MAP_HEIGHT);

    maze[endCell.second][endCell.first].grid[1][1] = 'e';

    return maze;
}

std::vector<std::vector<char>> mazeToChar(const std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT> &maze) {
    std::vector<std::vector<char>> cells;

    for (auto &row : maze) {
        std::vector<char> temp;
        for (auto &cell : row) {
            temp.emplace_back(cell.grid[0][0]);
            temp.emplace_back(cell.grid[0][1]);
        }
        temp.emplace_back('#');

        std::vector<char> temp2;
        for (auto &cell : row) {
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
