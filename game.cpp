#include "game.hpp"

#include <iostream>

#include "configuration.hpp"
#include "map.hpp"
#include "screens.hpp"

Game::Game()
        : m_window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Pacman Maze", sf::Style::Close) {
    if (!m_icon.loadFromFile("../Assets/Images/icon.png")) {
        throw std::invalid_argument("Wrong path to icon");
    }
    m_window.setIcon(m_icon.getSize().x, m_icon.getSize().y, m_icon.getPixelsPtr());
    m_window.setFramerateLimit(60);
    m_view = m_window.getDefaultView();
    m_window.setView(m_view);

    if (!m_font.loadFromFile("../Assets/Roboto-Black.ttf")) {
        throw std::invalid_argument("Wrong path to font");
    }
    Configuration::textConfig(m_timeText, m_font, "Time: 00.00s", 20, sf::Color(255, 102, 0));

    Configuration::loadTextures(m_textures);
    m_background.setTexture(m_textures.at("background"));
    m_background.setTextureRect(
            sf::IntRect(0, 0, static_cast<int>(2 * MAP_WIDTH * TILE), static_cast<int>(2 * MAP_HEIGHT * TILE)));

    Configuration::loadSoundBuffer(m_hitSoundBuffer);
}

void Game::runGame() {
    Pacman player(&m_textures.at("pacman"), 2, 0.15f, 200.0f, m_hitSoundBuffer);
    m_view.setCenter(player.getPosition());

    Map tileMap(m_textures.at("wall"), m_textures.at("start"), m_textures.at("end"), m_textures.at("point"));

    Screens::mainMenu(m_window, m_font);

    sf::Mouse::setPosition(sf::Vector2i(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), m_window);

    sf::Clock clock;
    unsigned int counter = 1;

    // game loop
    while (m_window.isOpen()) {
        // event loop
        sf::Event event;
        while (m_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                m_window.close();
            }
        }

        // Checking if cursor is inside the window
        if (m_window.hasFocus() &&
            sf::Mouse::getPosition(m_window).y > 0 &&
            sf::Mouse::getPosition(m_window).y < static_cast<int>(WINDOW_HEIGHT) &&
            sf::Mouse::getPosition(m_window).x > 0 &&
            sf::Mouse::getPosition(m_window).x < static_cast<int>(WINDOW_WIDTH)) {
            m_deltaTime = clock.restart().asSeconds();
        } else {
            clock.restart();
            m_deltaTime = 0.0f;
        }

        if (!m_endTileHit) {
            m_playingTime += m_deltaTime;
        }

        std::string strPlayingTime = std::to_string(m_playingTime);

        if (counter >= 10) {
            m_timeText.setString("Time: " + strPlayingTime.substr(0, strPlayingTime.find('.') + 3) + "s");
            counter = 1;
        } else {
            counter++;
        }

        sf::Vector2f mousePos(m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window)).x,
                              m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window)).y);
        player.update(m_deltaTime, mousePos);

        tileMap.collisionDetection(player, m_endTileHit);

        if (m_endTileHit) {
            Screens::finalScreen(m_window, m_font, strPlayingTime);
        }

        tileMap.getLight().checkVisibility(player, mousePos);

        m_view.setCenter(player.getPosition());
        m_timeText.setPosition(player.getPosition().x + 20.0f, player.getPosition().y);

        m_window.clear();

        m_window.setView(m_view);
        m_window.draw(m_background);
        tileMap.draw(m_window);
        tileMap.getLight().drawLight(player.getPosition(), m_window);
        m_window.draw(player);
        m_window.draw(m_timeText);

        m_window.display();
    }
}
