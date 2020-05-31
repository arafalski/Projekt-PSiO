#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include "pacman.hpp"
#include "consts.hpp"
#include "map.hpp"
#include "collider.hpp"
#include "screens.hpp"
#include "configuration.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Pacman Maze", sf::Style::Close);
    sf::Image icon;
    if (!icon.loadFromFile("../Assets/Images/icon.png")) {
        std::cerr << "Couldn't load an icon\n";
        return EXIT_FAILURE;
    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);
    sf::View view = window.getDefaultView();
    window.setView(view);

    sf::Font font;
    if (!font.loadFromFile("../Assets/Roboto-Black.ttf")) {
        std::cerr << "Couldn't load a font\n";
        return EXIT_FAILURE;
    }
    sf::Text timeText;
    Configuration::textConfig(timeText, font, "Time: 00.00s", 20, sf::Color(255, 102, 0));

    std::map<std::string, sf::Texture> textures;

    try {
        Configuration::loadTextures(textures);
    }
    catch (std::invalid_argument &except) {
        std::cerr << except.what() << '\n';
        return EXIT_FAILURE;
    }

    sf::Sprite background;
    background.setTexture(textures["background"]);
    background.setTextureRect(
            sf::IntRect(0, 0, static_cast<int>(2 * MAP_WIDTH * TILE), static_cast<int>(2 * MAP_HEIGHT * TILE)));

    sf::SoundBuffer hitSoundBuffer;
    try {
        Configuration::loadSoundBuffer(hitSoundBuffer);
    }
    catch (std::invalid_argument &except) {
        std::cerr << except.what() << '\n';
        return EXIT_FAILURE;
    }

    Pacman player(&textures["pacman"], sf::Vector2u(2, 1), 0.15f, 200.0f, hitSoundBuffer);
    view.setCenter(player.getPosition());

    Map tileMap(textures["wall"], textures["start"], textures["end"], textures["point"]);

    Screens::mainMenu(window, font);

    sf::Mouse::setPosition(sf::Vector2i(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), window);

    sf::Clock clock;
    float deltaTime;
    float playingTime = 0;
    unsigned int counter = 1;
    bool endTileHit = false;

    // game loop
    while (window.isOpen()) {

        // Checking if cursor is inside the window
        if (window.hasFocus() &&
            sf::Mouse::getPosition(window).y > 0 &&
            sf::Mouse::getPosition(window).y < static_cast<int>(WINDOW_HEIGHT) &&
            sf::Mouse::getPosition(window).x > 0 &&
            sf::Mouse::getPosition(window).x < static_cast<int>(WINDOW_WIDTH)) {
            deltaTime = clock.restart().asSeconds();
        } else {
            clock.restart();
            deltaTime = 0.0f;
        }

        if (!endTileHit) {
            playingTime += deltaTime;
        }

        std::string strPlayingTime = std::to_string(playingTime);

        if (counter >= 10) {
            timeText.setString("Time: " + strPlayingTime.substr(0, strPlayingTime.find('.') + 3) + "s");
            counter = 1;
        } else {
            counter++;
        }

        // event loop
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        sf::Vector2f mousePos(window.mapPixelToCoords(sf::Mouse::getPosition(window)).x,
                              window.mapPixelToCoords(sf::Mouse::getPosition(window)).y);
        player.update(deltaTime, mousePos);

        tileMap.collisionDetection(player, endTileHit);

        if (endTileHit) {
            Screens::finalScreen(window, icon, font, strPlayingTime);
        }

        tileMap.checkVisibility(player, mousePos);

        view.setCenter(player.getPosition());
        timeText.setPosition(player.getPosition().x + 20.0f, player.getPosition().y);
        window.clear();
        window.setView(view);
        window.draw(background);
        tileMap.draw(window);
        tileMap.drawLight(player.getPosition(), window);
        window.draw(player);
        window.draw(timeText);
        window.display();
    }

    return 0;
}
