#ifndef PROJEKT_SCREENS_HPP
#define PROJEKT_SCREENS_HPP

#include <string>

#include <SFML/Graphics.hpp>

class Screens {
public:
    static void mainMenu(sf::RenderWindow &window, const sf::Font &font);

    static void finalScreen(sf::RenderWindow &window, const sf::Font &font, const std::string &strPlayingTime);
};

#endif //PROJEKT_SCREENS_HPP
