#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "pacman.hpp"
#include "consts.hpp"
#include "map.hpp"
#include "collider.hpp"

void finalScreen(sf::RenderWindow &window, sf::Image &icon, sf::Font &font, const std::string &strPlayingTime) {
    window.create(sf::VideoMode(450, 200), "Pacman Maze", sf::Style::Close);
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    sf::Text victoryText;
    victoryText.setFont(font);
    victoryText.setString("Victory!!!");
    victoryText.setCharacterSize(100);
    victoryText.setFillColor(sf::Color::Red);
    victoryText.setPosition(static_cast<float>(window.getSize().x / 2) - victoryText.getGlobalBounds().width / 2, 0.0f);

    sf::Text timeText;
    timeText.setFont(font);
    timeText.setString("Time: " + strPlayingTime.substr(0, strPlayingTime.find('.') + 3) + "s");
    timeText.setCharacterSize(50);
    timeText.setFillColor(sf::Color::Green);
    timeText.setPosition(static_cast<float>(window.getSize().x / 2) - timeText.getGlobalBounds().width / 2,
                         victoryText.getGlobalBounds().height + 15);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        window.clear();
        window.draw(victoryText);
        window.draw(timeText);
        window.display();
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Pacman Maze", sf::Style::Close);
    sf::Image icon;
    if (!icon.loadFromFile("../Assets/Images/icon.png")) {
        return EXIT_FAILURE;
    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);
    sf::View view = window.getDefaultView();
    window.setView(view);

    sf::Font font;
    if (!font.loadFromFile("../Assets/Roboto-Black.ttf")) {
        return EXIT_FAILURE;
    }
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setString("Time: 00.00s");
    scoreText.setCharacterSize(20);
    scoreText.setFillColor(sf::Color(255, 102, 0));

    sf::Mouse::setPosition(sf::Vector2i(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), window);

    sf::Texture pacmanTexture;
    if (!pacmanTexture.loadFromFile("../Assets/Images/pacman.png")) {
        return EXIT_FAILURE;
    }
    Pacman player(&pacmanTexture, sf::Vector2u(2, 1), 0.15f, 200.0f);
    view.setCenter(player.getPosition());

    std::vector<sf::RectangleShape> obstacles = generateMap();

    sf::Clock clock;
    float deltaTime = 0;
    float playingTime = 0;
    unsigned int counter = 1;
    bool endTileHit = false;

    // game loop
    while (window.isOpen()) {
        deltaTime = clock.restart().asSeconds();
        if (!endTileHit) {
            playingTime += deltaTime;
        }

        std::string strPlayingTime = std::to_string(playingTime);

        if (counter >= 10) {
            scoreText.setString("Time: " + strPlayingTime.substr(0, strPlayingTime.find('.') + 3) + "s");
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

        player.update(deltaTime, sf::Vector2f(window.mapPixelToCoords(sf::Mouse::getPosition(window)).x,
                                              window.mapPixelToCoords(sf::Mouse::getPosition(window)).y));
        sf::Vector2f direction;
        Collider playerCollider = player.getCollider();

        for (auto &box : obstacles) {
            if (box.getFillColor() == sf::Color::Blue && Collider(box).checkCollision(playerCollider, direction)) {
                player.onCollision(direction);
            } else if (box.getFillColor() == sf::Color::Red &&
                       box.getGlobalBounds().intersects(player.getGlobalBounds())) {
                endTileHit = true;
            } else if (box.getFillColor() == sf::Color::Yellow &&
                       Collider(box).checkCollision(playerCollider, direction)) {
                box.setFillColor(sf::Color::Black);
            }
        }

        if (endTileHit) {
            finalScreen(window, icon, font, strPlayingTime);
        }

        view.setCenter(player.getPosition());
        scoreText.setPosition(player.getPosition().x + 20.0f, player.getPosition().y);
        window.clear();
        window.setView(view);
        for (auto &box : obstacles) {
            window.draw(box);
        }
        window.draw(player);
        window.draw(scoreText);
        window.display();
    }

    return 0;
}
