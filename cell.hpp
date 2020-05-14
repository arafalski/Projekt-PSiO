#ifndef PROJEKT_CELL_HPP
#define PROJEKT_CELL_HPP

#include <array>

class Cell {
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

private:
    bool visited = false;
};

#endif //PROJEKT_CELL_HPP
