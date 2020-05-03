#ifndef PROJEKT_CONFIGURATION_HPP
#define PROJEKT_CONFIGURATION_HPP

#include <SFML/Graphics.hpp>

inline void
textConfig(sf::Text &text, const sf::Font &font, const std::string &textToWrite, const unsigned int &characterSize,
           const sf::Color &color) {
    text.setFont(font);
    text.setString(textToWrite);
    text.setCharacterSize(characterSize);
    text.setFillColor(color);
}

#endif //PROJEKT_CONFIGURATION_HPP
