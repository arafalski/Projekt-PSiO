#ifndef PROJEKT_GAME_HPP
#define PROJEKT_GAME_HPP

#include <map>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "pacman.hpp"

class Game {
public:
    Game();

    void runGame();

private:
    sf::RenderWindow m_window;
    sf::View m_view;

    sf::Image m_icon;
    sf::Font m_font;
    sf::Text m_timeText;
    sf::Sprite m_background;
    sf::SoundBuffer hitSoundBuffer;
    std::map<std::string, sf::Texture> m_textures;

    float m_deltaTime;
    float m_playingTime = 0.0f;
    bool m_endTileHit = false;
};


#endif //PROJEKT_GAME_HPP
