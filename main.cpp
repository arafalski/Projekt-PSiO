#include <iostream>

#include "game.hpp"

int main() {
    try {
        Game game;
        game.runGame();
    }
    catch (std::out_of_range& except) {
        std::cerr << "There is no such a texture: " << except.what() << '\n';
        return 1;
    }
    catch (std::invalid_argument& except) {
        std::cerr << "Invalid path: " << except.what() << '\n';
        return 1;
    }
    catch (...) {
        std::cerr << "Unknown exception!!!\n";
    }

    return 0;
}
