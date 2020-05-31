#ifndef PROJEKT_SCREENS_HPP
#define PROJEKT_SCREENS_HPP

#include <SFML/Graphics.hpp>
#include <string>

class Screens {
public:
    static void mainMenu(sf::RenderWindow &window, const sf::Font &font);

    static void finalScreen(sf::RenderWindow &window, const sf::Font &font, const std::string &strPlayingTime);
};

#endif //PROJEKT_SCREENS_HPP
