#ifndef PROJEKT_CONFIGURATION_HPP
#define PROJEKT_CONFIGURATION_HPP

#include <SFML/Graphics.hpp>
#include <map>

class Configuration {
public:
    static void
    textConfig(sf::Text &text, const sf::Font &font, const std::string &textToWrite, const unsigned int &characterSize,
               const sf::Color &color);

    static void loadTextures(std::map<std::string, sf::Texture> &textureMap);
};

#endif //PROJEKT_CONFIGURATION_HPP
