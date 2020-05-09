#include "map.hpp"

Map::Map() {
    generate();
}

void Map::generate() {
    std::vector<std::vector<char>> cells = mazeToChar(generateTilesPlacement());
    std::thread convToPolyThread(&Map::convertTileMapToPolyMap, this, cells);

    for (std::size_t i = 0; i < cells.size(); i++) {
        for (std::size_t j = 0; j < cells[i].size(); j++) {
            sf::RectangleShape box;
            box.setSize(sf::Vector2f(TILE, TILE));
            box.setPosition(static_cast<float>(j) * TILE, static_cast<float>(i) * TILE);
            box.setOrigin(TILE / 2.0f, TILE / 2.0f);
            switch (cells[i][j]) {
                case '#':
                    box.setFillColor(sf::Color::Blue);
                    mapGrid.emplace_back(box);
                    break;
                case 's':
                    box.setFillColor(sf::Color::Green);
                    mapGrid.emplace_back(box);
                    break;
                case 'e':
                    box.setFillColor(sf::Color::Red);
                    mapGrid.emplace_back(box);
                    break;
                default:
                    box.setFillColor(sf::Color::Yellow);
                    box.setSize(sf::Vector2f(TILE / 4.0f, TILE / 4.0f));
                    box.setOrigin(TILE / 8.0f, TILE / 8.0f);
                    mapGrid.emplace_back(box);
                    break;
            }
        }
    }
    convToPolyThread.join();
}

std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT> Map::generateTilesPlacement() {
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

std::vector<std::vector<char>> Map::mazeToChar(const std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT> &maze) {
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

void Map::draw(sf::RenderWindow &window) const {
    for (const auto &cell : mapGrid) {
        window.draw(cell);
    }

    for (const auto &e : edges) {
        sf::CircleShape startPoint(5.0f);
        startPoint.setPosition(e.start_x, e.start_y);
        startPoint.setOrigin(startPoint.getRadius(), startPoint.getRadius());
        startPoint.setFillColor(sf::Color::Red);
        sf::CircleShape endPoint(5.0f);
        endPoint.setOrigin(endPoint.getRadius(), endPoint.getRadius());
        endPoint.setPosition(e.end_x, e.end_y);
        endPoint.setFillColor(sf::Color::Red);

        window.draw(startPoint);
        window.draw(endPoint);
    }
}

void Map::collisionDetection(Pacman &player, bool &endTileHit) {
    sf::Vector2f direction;
    Collider playerCollider = player.getCollider();

    for (auto &box : mapGrid) {
        if (box.getFillColor() == sf::Color::Blue && Collider(box).checkCollision(playerCollider, direction)) {
            player.onCollision(direction);
        } else if (box.getFillColor() == sf::Color::Red && box.getGlobalBounds().intersects(player.getGlobalBounds())) {
            endTileHit = true;
        } else if (box.getFillColor() == sf::Color::Yellow &&
                   Collider(box).checkCollision(playerCollider, direction)) {
            box.setFillColor(sf::Color::Black);
        }
    }
}

void Map::convertTileMapToPolyMap(const std::vector<std::vector<char>> &cellInChars) {
    std::mutex m;
    const std::lock_guard<std::mutex> lock(m);
    std::vector<std::vector<polyCell>> cells;

    for (const auto &row : cellInChars) {
        std::vector<polyCell> temp;
        for (const auto &x :row) {
            polyCell cell;
            if (x == '#') cell.exist = true;
            temp.emplace_back(cell);
        }
        cells.emplace_back(temp);
    }

    for (unsigned int i = 1; i < cells.size() - 1; i++) {
        for (unsigned int j = 1; j < cells[i].size() - 1; j++) {

            if (cells[i][j].exist) {
                // If cell has no western neighbour
                if (!cells[i][j - 1].exist) {
                    // It can extend an edge from its northern neighbour if it has one,
                    // or it can start a new one
                    if (cells[i - 1][j].edge[Direction::WEST].second) {
                        edges[cells[i - 1][j].edge[Direction::WEST].first].end_y += TILE;
                        cells[i][j].edge[Direction::WEST].first = cells[i - 1][j].edge[Direction::WEST].first;
                        cells[i][j].edge[Direction::WEST].second = true;
                    } else {
                        Edge edge;
                        edge.start_x = static_cast<float>(j) * TILE - TILE / 2;
                        edge.start_y = static_cast<float>(i) * TILE - TILE / 2;
                        edge.end_x = edge.start_x;
                        edge.end_y = edge.start_y + TILE;

                        std::size_t edgeID = edges.size();
                        edges.emplace_back(edge);

                        cells[i][j].edge[Direction::WEST].first = edgeID;
                        cells[i][j].edge[Direction::WEST].second = true;
                    }
                }

                // If cell has no eastern neighbour
                if (!cells[i][j + 1].exist) {
                    if (cells[i - 1][j].edge[Direction::EAST].second) {
                        edges[cells[i - 1][j].edge[Direction::EAST].first].end_y += TILE;
                        cells[i][j].edge[Direction::EAST].first = cells[i - 1][j].edge[Direction::EAST].first;
                        cells[i][j].edge[Direction::EAST].second = true;
                    } else {
                        Edge edge;
                        edge.start_x = static_cast<float>(j + 1) * TILE - TILE / 2;
                        edge.start_y = static_cast<float>(i) * TILE - TILE / 2;
                        edge.end_x = edge.start_x;
                        edge.end_y = edge.start_y + TILE;

                        std::size_t edgeID = edges.size();
                        edges.emplace_back(edge);

                        cells[i][j].edge[Direction::EAST].first = edgeID;
                        cells[i][j].edge[Direction::EAST].second = true;
                    }
                }

                // If cell has no northern neighbour
                if (!cells[i - 1][j].exist) {
                    if (cells[i][j - 1].edge[Direction::NORTH].second) {
                        edges[cells[i][j - 1].edge[Direction::NORTH].first].end_x += TILE;
                        cells[i][j].edge[Direction::NORTH].first = cells[i][j - 1].edge[Direction::NORTH].first;
                        cells[i][j].edge[Direction::NORTH].second = true;
                    } else {
                        Edge edge;
                        edge.start_x = static_cast<float>(j) * TILE - TILE / 2;
                        edge.start_y = static_cast<float>(i) * TILE - TILE / 2;
                        edge.end_x = edge.start_x + TILE;
                        edge.end_y = edge.start_y;

                        std::size_t edgeID = edges.size();
                        edges.emplace_back(edge);

                        cells[i][j].edge[Direction::NORTH].first = edgeID;
                        cells[i][j].edge[Direction::NORTH].second = true;
                    }
                }

                // If cell has no southern neighbour
                if (!cells[i + 1][j].exist) {
                    if (cells[i][j - 1].edge[Direction::SOUTH].second) {
                        edges[cells[i][j - 1].edge[Direction::SOUTH].first].end_x += TILE;
                        cells[i][j].edge[Direction::SOUTH].first = cells[i][j - 1].edge[Direction::SOUTH].first;
                        cells[i][j].edge[Direction::SOUTH].second = true;
                    } else {
                        Edge edge;
                        edge.start_x = static_cast<float>(j) * TILE - TILE / 2;
                        edge.start_y = static_cast<float>(i + 1) * TILE - TILE / 2;
                        edge.end_x = edge.start_x + TILE;
                        edge.end_y = edge.start_y;

                        std::size_t edgeID = edges.size();
                        edges.emplace_back(edge);

                        cells[i][j].edge[Direction::SOUTH].first = edgeID;
                        cells[i][j].edge[Direction::SOUTH].second = true;
                    }
                }
            }
        }
    }
}
