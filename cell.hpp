#ifndef PROJEKT_CELL_HPP
#define PROJEKT_CELL_HPP

#include <array>

class Cell {
    bool visited = false;

public:
    std::array<std::array<char, 2>, 2> grid{{{'#', '#'}, {'#', ' '}}};

    Cell() = default;

    ~Cell() = default;

    void visit() {
        visited = true;
    }

    bool wasVisited() const {
        return visited;
    }
};

#endif //PROJEKT_CELL_HPP
