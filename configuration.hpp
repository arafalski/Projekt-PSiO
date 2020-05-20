#ifndef PROJEKT_CONFIGURATION_HPP
#define PROJEKT_CONFIGURATION_HPP

#include <SFML/Graphics.hpp>
#include <stdexcept>
#include <map>

void textConfig(sf::Text &text, const sf::Font &font, const std::string &textToWrite, const unsigned int &characterSize,
                const sf::Color &color) {
    text.setFont(font);
    text.setString(textToWrite);
    text.setCharacterSize(characterSize);
    text.setFillColor(color);
}

void loadTextures(std::map<std::string, sf::Texture> &textureMap) {
    textureMap.insert({"pacman", sf::Texture()});
    if (!textureMap.at("pacman").loadFromFile("../Assets/Images/pacman.png")) {
        throw std::invalid_argument("Couldn't load a pacman's texture");
    }

    textureMap.insert({"wall", sf::Texture()});
    textureMap.at("wall").setRepeated(true);
    if (!textureMap.at("wall").loadFromFile("../Assets/Images/grayTile.png")) {
        throw std::invalid_argument("Couldn't load a wall texture");
    }

    textureMap.insert({"start", sf::Texture()});
    if (!textureMap.at("start").loadFromFile("../Assets/Images/greenTile.png")) {
        throw std::invalid_argument("Couldn't load a starting tile texture");
    }

    textureMap.insert({"end", sf::Texture()});
    if (!textureMap.at("end").loadFromFile("../Assets/Images/redTile.png")) {
        throw std::invalid_argument("Couldn't load a ending tile texture");
    }

    textureMap.insert({"point", sf::Texture()});
    if (!textureMap.at("point").loadFromFile("../Assets/Images/yellowTile.png")) {
        throw std::invalid_argument("Couldn't load a point texture");
    }

    textureMap.insert({"background", sf::Texture()});
    textureMap.at("background").setRepeated(true);
    if (!textureMap.at("background").loadFromFile("../Assets/Images/darkGrayTile.png")) {
        throw std::invalid_argument("Couldn't load a background texture");
    }
}

#endif //PROJEKT_CONFIGURATION_HPP
