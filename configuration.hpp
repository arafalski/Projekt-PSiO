#ifndef PROJEKT_CONFIGURATION_HPP
#define PROJEKT_CONFIGURATION_HPP

#include <SFML/Graphics.hpp>
#include <stdexcept>

void textConfig(sf::Text &text, const sf::Font &font, const std::string &textToWrite, const unsigned int &characterSize,
                const sf::Color &color) {
    text.setFont(font);
    text.setString(textToWrite);
    text.setCharacterSize(characterSize);
    text.setFillColor(color);
}

void textureLoading(sf::Texture &textureToSet, const std::string &pathToTexture){
    if(!textureToSet.loadFromFile(pathToTexture)){
        throw std::invalid_argument("Couldn't load a texture");
    }
}

#endif //PROJEKT_CONFIGURATION_HPP
