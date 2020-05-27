#include "configuration.hpp"
#include <stdexcept>

void Configuration::textConfig(sf::Text &text, const sf::Font &font, const std::string &textToWrite,
                               const unsigned int &characterSize,
                               const sf::Color &color) {
    text.setFont(font);
    text.setString(textToWrite);
    text.setCharacterSize(characterSize);
    text.setFillColor(color);
}

void Configuration::loadTextures(std::map<std::string, sf::Texture> &textureMap) {
    textureMap.insert({"pacman", sf::Texture()});
    if (!textureMap["pacman"].loadFromFile("../Assets/Images/pacman.png")) {
        throw std::invalid_argument("Couldn't load a pacman's texture");
    }

    textureMap.insert({"wall", sf::Texture()});
    textureMap["wall"].setRepeated(true);
    if (!textureMap["wall"].loadFromFile("../Assets/Images/grayTile.png")) {
        throw std::invalid_argument("Couldn't load a wall texture");
    }

    textureMap.insert({"start", sf::Texture()});
    if (!textureMap["start"].loadFromFile("../Assets/Images/greenTile.png")) {
        throw std::invalid_argument("Couldn't load a starting tile texture");
    }

    textureMap.insert({"end", sf::Texture()});
    if (!textureMap["end"].loadFromFile("../Assets/Images/redTile.png")) {
        throw std::invalid_argument("Couldn't load a ending tile texture");
    }

    textureMap.insert({"point", sf::Texture()});
    if (!textureMap["point"].loadFromFile("../Assets/Images/yellowTile.png")) {
        throw std::invalid_argument("Couldn't load a point texture");
    }

    textureMap.insert({"background", sf::Texture()});
    textureMap["background"].setRepeated(true);
    if (!textureMap["background"].loadFromFile("../Assets/Images/darkGrayTile.png")) {
        throw std::invalid_argument("Couldn't load a background texture");
    }
}

void Configuration::loadSoundBuffer(sf::SoundBuffer &soundBufferToLoad) {
    if (!soundBufferToLoad.loadFromFile("../Assets/Audio/impact.ogg")) {
        throw std::invalid_argument("Couldn't load audio file");
    }
}
