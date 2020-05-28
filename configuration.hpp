#ifndef PROJEKT_CONFIGURATION_HPP
#define PROJEKT_CONFIGURATION_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <map>

class Configuration {
public:
    static void
    textConfig(sf::Text &text, const sf::Font &font, const std::string &textToWrite, unsigned int characterSize,
               const sf::Color &color);

    static void loadTextures(std::map<std::string, sf::Texture> &textureMap);

    static void loadSoundBuffer(sf::SoundBuffer &soundBufferToLoad);
};

#endif //PROJEKT_CONFIGURATION_HPP
