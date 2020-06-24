#ifndef PROJEKT_LIGHT_HPP
#define PROJEKT_LIGHT_HPP

#include <map>
#include <vector>

#include <SFML/Graphics.hpp>
#include "consts.hpp"

namespace {
    constexpr auto MAX_RADIUS = 200.f;
}

class Light {
public:
    Light() = default;

    explicit Light(const std::vector<std::vector<char>>& cellsInChars);

    void checkVisibility(const sf::Vector2f& playerPos, const sf::Vector2f& mousePos);

    void drawLight(const sf::Vector2f& playerPos, sf::RenderWindow& window);

private:
    struct Edge {
        sf::Vector2f start;
        sf::Vector2f end;
    };

    struct polyCell {
        std::map<Direction, std::pair<size_t, bool>> edge{{Direction::NORTH, {0, false}},
                                                          {Direction::SOUTH, {0, false}},
                                                          {Direction::EAST,  {0, false}},
                                                          {Direction::WEST,  {0, false}}};
        bool exist = false;
    };

    std::vector<Edge> m_edges;

    std::vector<std::pair<float, sf::Vector2f>> m_visiblePolyPoints;

    void addPolyMapBoundary();

    float angleCount(sf::Vector2f vec);

    void checkIntersection(float angle, const sf::Vector2f& playerPos);

    void sortAndEraseDuplicatesVisiblePoints();
};


#endif //PROJEKT_LIGHT_HPP
