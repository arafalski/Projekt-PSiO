#include "map.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <limits>
#include <map>
#include <mutex>
#include <random>
#include <stack>
#include <thread>

Map::Map(sf::Texture& wallTexture, sf::Texture& startTexture, sf::Texture& endTexture, sf::Texture& pointTexture) {
    std::vector<std::vector<char>> cells = mazeToChar(generateTilesPlacement());
    std::thread convToPolyThread(&Map::convertTileMapToPolyMap, this, cells);

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
    convToPolyThread.join();
}

std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT> Map::generateTilesPlacement() {
    std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT> maze{};
    std::stack<sf::Vector2u> backtrack;
    unsigned int visitedCells;
    std::mt19937 generator(
            static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));

    std::size_t longestPath = 0;
    sf::Vector2u endCell;

    backtrack.push(sf::Vector2u(0, 0));
    visitedCells = 1;
    maze[0][0].visit();
    maze[0][0].grid[1][1] = 's';

    do {
        std::vector<Direction> neighbours;

        if (backtrack.top().y > 0 && !maze[backtrack.top().y - 1][backtrack.top().x].wasVisited()) {
            neighbours.emplace_back(Direction::NORTH);
        }
        if (backtrack.top().y < MAP_HEIGHT - 1 && !maze[backtrack.top().y + 1][backtrack.top().x].wasVisited()) {
            neighbours.emplace_back(Direction::SOUTH);
        }
        if (backtrack.top().x < MAP_WIDTH - 1 && !maze[backtrack.top().y][backtrack.top().x + 1].wasVisited()) {
            neighbours.emplace_back(Direction::EAST);
        }
        if (backtrack.top().x > 0 && !maze[backtrack.top().y][backtrack.top().x - 1].wasVisited()) {
            neighbours.emplace_back(Direction::WEST);
        }

        if (!neighbours.empty()) {
            Direction nextDir = neighbours[generator() % neighbours.size()];
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
            maze[backtrack.top().y][backtrack.top().x].visit();
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

    for (auto& row : maze) {
        std::vector<char> temp;
        for (auto& cell : row) {
            temp.emplace_back(cell.grid[0][0]);
            temp.emplace_back(cell.grid[0][1]);
        }
        temp.emplace_back('#');

        std::vector<char> temp2;
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
    Collider playerCollider = player.getCollider();

    auto hitPointIt = m_mapGrid.end();
    bool hitWall = false;
    for (std::size_t i = 0; i < m_mapGrid.size(); i++) {
        Collider boxCollider = m_mapGrid[i].getCollider();

        if (m_mapGrid[i].getFunction() == '#' && boxCollider.checkCollision(playerCollider, dir)) {
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

void Map::addPolyMapBoundary() {
    Edge Top, Right, Bottom, Left;

    Top.start.x = TILE / 2.0f;
    Top.start.y = TILE / 2.0f;
    Top.end.x = Top.start.x + 2 * TILE * (MAP_WIDTH - 0.5f);
    Top.end.y = Top.start.y;

    Right.start.x = Top.end.x;
    Right.start.y = Top.end.y;
    Right.end.x = Right.start.x;
    Right.end.y = Right.start.y + 2 * TILE * (MAP_HEIGHT - 0.5f);

    Bottom.start.x = Right.end.x;
    Bottom.start.y = Right.end.y;
    Bottom.end.x = Bottom.start.x - 2 * TILE * (MAP_WIDTH - 0.5f);
    Bottom.end.y = Bottom.start.y;

    Left.start.x = Bottom.end.x;
    Left.start.y = Bottom.end.y;
    Left.end.x = Left.start.x;
    Left.end.y = Left.start.y - 2 * TILE * (MAP_HEIGHT - 0.5f);

    m_edges.emplace_back(Top);
    m_edges.emplace_back(Right);
    m_edges.emplace_back(Bottom);
    m_edges.emplace_back(Left);
}

void Map::convertTileMapToPolyMap(const std::vector<std::vector<char>>& cellInChars) {
    std::mutex m;
    const std::lock_guard<std::mutex> lock(m);
    std::vector<std::vector<polyCell>> cells;

    for (const auto& row : cellInChars) {
        std::vector<polyCell> temp;
        for (const auto x :row) {
            polyCell cell;
            if (x == '#') cell.exist = true;
            temp.emplace_back(cell);
        }
        cells.emplace_back(temp);
    }

    addPolyMapBoundary();

    for (unsigned int i = 1; i < cells.size() - 1; i++) {
        for (unsigned int j = 1; j < cells[i].size() - 1; j++) {

            auto checkEdge = [&](const auto& edgeOwner, const auto& startOffset, const auto& endOffset,
                                 const auto dir) {
                if (cells[edgeOwner.y][edgeOwner.x].edge[dir].second) {
                    if (dir == Direction::WEST || dir == Direction::EAST) {
                        m_edges[cells[edgeOwner.y][edgeOwner.x].edge[dir].first].end.y += TILE;
                    } else { m_edges[cells[edgeOwner.y][edgeOwner.x].edge[dir].first].end.x += TILE; };
                    cells[i][j].edge[dir].first = cells[edgeOwner.y][edgeOwner.x].edge[dir].first;
                    cells[i][j].edge[dir].second = true;
                } else {
                    Edge edge;
                    edge.start.x = static_cast<float>(startOffset.x) * TILE - TILE / 2;
                    edge.start.y = static_cast<float>(startOffset.y) * TILE - TILE / 2;
                    edge.end.x = edge.start.x + TILE * endOffset.x;
                    edge.end.y = edge.start.y + TILE * endOffset.y;

                    m_edges.emplace_back(edge);
                    cells[i][j].edge[dir].first = m_edges.size() - 1;
                    cells[i][j].edge[dir].second = true;
                }
            };

            if (cells[i][j].exist) {
                if (!cells[i][j - 1].exist) {
                    checkEdge(sf::Vector2u(j, i - 1), sf::Vector2u(j, i), sf::Vector2f(0, 1), Direction::WEST);
                }

                if (!cells[i][j + 1].exist) {
                    checkEdge(sf::Vector2u(j, i - 1), sf::Vector2u(j + 1, i), sf::Vector2f(0, 1), Direction::EAST);
                }

                if (!cells[i - 1][j].exist) {
                    checkEdge(sf::Vector2u(j - 1, i), sf::Vector2u(j, i), sf::Vector2f(1, 0), Direction::NORTH);
                }

                if (!cells[i + 1][j].exist) {
                    checkEdge(sf::Vector2u(j - 1, i), sf::Vector2u(j, i + 1), sf::Vector2f(1, 0), Direction::SOUTH);
                }
            }
        }
    }
}

float Map::angleCount(sf::Vector2f vec) {
    vec.y *= -1;

    float angle;
    if (vec.y >= 0) {
        angle = atan2f(vec.y, vec.x) * 180.0f / static_cast<float>(M_PI);
    } else {
        angle = 360.0f + atan2f(vec.y, vec.x) * 180.0f / static_cast<float>(M_PI);
    }

    return angle;
}

void Map::checkVisibility(const Pacman& player, const sf::Vector2f& mousePos) {
    m_visiblePolyPoints.clear();
    auto playerPos = player.getPosition();
    auto toCursor = mousePos - playerPos;

    sf::Vector2f firstBorder(toCursor.x - toCursor.y, toCursor.x + toCursor.y);
    sf::Vector2f secondBorder(toCursor.x + toCursor.y, toCursor.y - toCursor.x);

    auto firstAngle = angleCount(firstBorder);
    auto secondAngle = angleCount(secondBorder);

    std::vector<float> visibleAngles{firstAngle, secondAngle};

    for (const auto& edge : m_edges) {
        for (int i = 0; i < 2; i++) {
            sf::Vector2f point((i == 0 ? edge.start.x : edge.end.x) - playerPos.x,
                               (i == 0 ? edge.start.y : edge.end.y) - playerPos.y);

            auto base_angle = angleCount(point);
            if (firstAngle > 270.0f && secondAngle < 90.0f) {
                if (base_angle < secondAngle || base_angle > firstAngle) {
                    visibleAngles.emplace_back(base_angle);
                }
            } else {
                if (base_angle < secondAngle && base_angle > firstAngle) {
                    visibleAngles.emplace_back(base_angle);
                }
            }
        }
    }

    for (const auto& angle : visibleAngles) {
        checkIntersection(angle, playerPos);
    }

    sortAndEraseDuplicatesVisiblePoints();
}

void Map::checkIntersection(float angle, const sf::Vector2f& playerPos) {
    for (int j = 0; j < 3; j++) {
        if (j == 1) angle -= 0.0001f;
        if (j == 2) angle += 0.0002f;

        sf::Vector2f ray;
        ray.x = cosf(angle * static_cast<float>(M_PI) / 180.0f);
        ray.y = -sinf(angle * static_cast<float>(M_PI) / 180.0f);

        auto min_t1 = std::numeric_limits<float>::infinity();
        sf::Vector2f rayEndPoint;
        float rayAngle;
        bool hitSomething = false;

        for (const auto& edge2 : m_edges) {
            sf::Vector2f segment(edge2.end.x - edge2.start.x,
                                 edge2.end.y - edge2.start.y);

            if (std::abs(segment.x - ray.x) > 0.0f && std::abs(segment.y - ray.y) > 0.0f) {
                float t2 =
                        (ray.x * (edge2.start.y - playerPos.y) + (ray.y * (playerPos.x - edge2.start.x))) /
                        (segment.x * ray.y - segment.y * ray.x);
                float t1 = (edge2.start.x + segment.x * t2 - playerPos.x) / ray.x;

                if (t1 > 0.0f && t2 >= 0.0f && t2 <= 1.0f) {
                    if (t1 < min_t1) {
                        min_t1 = t1;
                        rayEndPoint.x = playerPos.x + ray.x * t1;
                        rayEndPoint.y = playerPos.y + ray.y * t1;
                        rayAngle = angleCount(rayEndPoint - playerPos);
                        hitSomething = true;
                    }
                }
            }
        }
        if (hitSomething) {
            m_visiblePolyPoints.emplace_back(std::make_pair(rayAngle, rayEndPoint));
        }
    }
}

void Map::sortAndEraseDuplicatesVisiblePoints() {
    auto sortAngles = [](const auto& lhs, const auto& rhs) {
        return lhs.first < rhs.first;
    };

    if (m_visiblePolyPoints[0].first >= 260.0f && m_visiblePolyPoints[4].first <= 100.0f) {
        auto firstConvex = std::partition(m_visiblePolyPoints.begin(), m_visiblePolyPoints.end(),
                                          [](const auto& el) { return el.first > 180.0f; });

        std::sort(m_visiblePolyPoints.begin(), firstConvex, sortAngles);
        std::sort(firstConvex, m_visiblePolyPoints.end(), sortAngles);
    } else {
        std::sort(m_visiblePolyPoints.begin(), m_visiblePolyPoints.end(), sortAngles);
    }

    auto it = std::unique(m_visiblePolyPoints.begin(), m_visiblePolyPoints.end(),
                          [](const auto& p1, const auto& p2) {
                              return std::abs(p1.second.x - p2.second.x) < 0.1f &&
                                     std::abs(p1.second.y - p2.second.y) < 0.1f;
                          });

    m_visiblePolyPoints.erase(it, m_visiblePolyPoints.end());
}

void Map::drawLight(const sf::Vector2f& playerPos, sf::RenderWindow& window) {
    sf::VertexArray light(sf::TriangleFan, m_visiblePolyPoints.size() + 1);
    uint8_t r = 255, g = 202, b = 3, alpha = 200;

    if (!m_visiblePolyPoints.empty()) {
        light[0].position = playerPos;
        light[0].color = sf::Color(r, g, b, alpha);

        for (std::size_t i = 0; i < m_visiblePolyPoints.size(); i++) {
            float distToPlayer = sqrtf(powf(m_visiblePolyPoints[i].second.x - playerPos.x, 2) +
                                       powf(m_visiblePolyPoints[i].second.y - playerPos.y, 2));

            light[i + 1].position = m_visiblePolyPoints[i].second;
            light[i + 1].color = sf::Color(r, g, b, static_cast<uint8_t>(log10f(distToPlayer / 100) / alpha));
        }
    }

    window.draw(light);
}
