#include "light.hpp"

#include <cmath>

Light::Light(const std::vector<std::vector<char>>& cellsInChars) {
    std::vector<std::vector<polyCell>> cells;

    for (const auto& row : cellsInChars) {
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
                    } else {
                        m_edges[cells[edgeOwner.y][edgeOwner.x].edge[dir].first].end.x += TILE;
                    };
                    cells[i][j].edge[dir].first = cells[edgeOwner.y][edgeOwner.x].edge[dir].first;
                    cells[i][j].edge[dir].second = true;
                } else {
                    m_edges.emplace_back(Edge());
                    m_edges.back().start.x = static_cast<float>(startOffset.x) * TILE - TILE / 2;
                    m_edges.back().start.y = static_cast<float>(startOffset.y) * TILE - TILE / 2;
                    m_edges.back().end.x = m_edges.back().start.x + TILE * endOffset.x;
                    m_edges.back().end.y = m_edges.back().start.y + TILE * endOffset.y;

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

void Light::addPolyMapBoundary() {
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

float Light::angleCount(sf::Vector2f vec) {
    vec.y *= -1;

    float angle;
    if (vec.y >= 0) {
        angle = atan2f(vec.y, vec.x) * 180.0f / static_cast<float>(M_PI);
    } else {
        angle = 360.0f + atan2f(vec.y, vec.x) * 180.0f / static_cast<float>(M_PI);
    }

    return angle;
}

void Light::checkVisibility(const Pacman& player, const sf::Vector2f& mousePos) {
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

void Light::checkIntersection(float angle, const sf::Vector2f& playerPos) {
    for (int j = 0; j < 3; j++) {
        if (j == 1) angle -= 0.0001f;
        if (j == 2) angle += 0.0002f;

        sf::Vector2f ray(cosf(angle * static_cast<float>(M_PI) / 180.0f),
                         -sinf(angle * static_cast<float>(M_PI) / 180.0f));

        auto minT1 = std::numeric_limits<float>::infinity();
        sf::Vector2f rayEndPoint;
        float rayAngle;
        bool hitSomething = false;

        for (const auto& edge : m_edges) {
            sf::Vector2f segment(edge.end.x - edge.start.x,
                                 edge.end.y - edge.start.y);

            if (std::abs(segment.x - ray.x) > 0.0f && std::abs(segment.y - ray.y) > 0.0f) {
                auto t2 = (ray.x * (edge.start.y - playerPos.y) + (ray.y * (playerPos.x - edge.start.x)))
                           / (ray.y * segment.x - ray.x * segment.y);
                auto t1 = (edge.start.x - playerPos.x + segment.x * t2) / ray.x;

                if (t1 > 0.0f && t2 > 0.0f && t2 < 1.0f && t1 < minT1) {
                    minT1 = t1;
                    rayEndPoint.x = playerPos.x + ray.x * t1;
                    rayEndPoint.y = playerPos.y + ray.y * t1;
                    rayAngle = angleCount(rayEndPoint - playerPos);
                    hitSomething = true;
                }
            }
        }
        if (hitSomething) {
            m_visiblePolyPoints.emplace_back(rayAngle, rayEndPoint);
        }
    }
}

void Light::sortAndEraseDuplicatesVisiblePoints() {
    auto sortAngles = [](const auto& lhs, const auto& rhs) {
        return lhs.first < rhs.first;
    };

    if (m_visiblePolyPoints[0].first > 270.0f && m_visiblePolyPoints[4].first < 90.0f) {
        auto firstConvex = std::partition(m_visiblePolyPoints.begin(), m_visiblePolyPoints.end(), [](const auto& el) {
            return el.first > 180.0f;
        });

        std::sort(m_visiblePolyPoints.begin(), firstConvex, sortAngles);
        std::sort(firstConvex, m_visiblePolyPoints.end(), sortAngles);
    } else {
        std::sort(m_visiblePolyPoints.begin(), m_visiblePolyPoints.end(), sortAngles);
    }

    auto it = std::unique(m_visiblePolyPoints.begin(), m_visiblePolyPoints.end(), [](const auto& lhs, const auto& rhs) {
        return std::abs(lhs.second.x - rhs.second.x) < 0.1f &&
               std::abs(lhs.second.y - rhs.second.y) < 0.1f;
    });

    m_visiblePolyPoints.erase(it, m_visiblePolyPoints.end());
}

void Light::drawLight(const sf::Vector2f& playerPos, sf::RenderWindow& window) {
    sf::VertexArray light(sf::TriangleFan, m_visiblePolyPoints.size() + 1);
    uint8_t r = 255, g = 202, b = 3, alpha = 250;

    if (!m_visiblePolyPoints.empty()) {
        light[0].position = playerPos;
        light[0].color = sf::Color(r, g, b, alpha);

        for (std::size_t i = 0; i < m_visiblePolyPoints.size(); i++) {
            auto distToPlayer = sqrtf(powf(m_visiblePolyPoints[i].second.x - playerPos.x, 2) +
                                      powf(m_visiblePolyPoints[i].second.y - playerPos.y, 2));

            light[i + 1].position = m_visiblePolyPoints[i].second;
            auto newAlpha = static_cast<float>(-3.f * distToPlayer + alpha);
            if (newAlpha < 0) {
                newAlpha = 0.0f;
            }
            light[i + 1].color = sf::Color(r, g, b, static_cast<uint8_t>(newAlpha));
        }
    }

    window.draw(light);
}
