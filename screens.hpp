#ifndef PROJEKT_SCREENS_HPP
#define PROJEKT_SCREENS_HPP

#include <SFML/Graphics.hpp>
#include "configuration.hpp"

void mainMenu(sf::RenderWindow &window, const sf::Font &font) {
    sf::Text gameName;
    textConfig(gameName, font, "Pacman Maze", 100, sf::Color::Green);
    gameName.setPosition(static_cast<float>(window.getSize().x) / 2.0f - gameName.getGlobalBounds().width / 2.0f,
                         0.0f);

    sf::RectangleShape playButton;
    playButton.setSize(sf::Vector2f(200.0f, 75.0f));
    playButton.setOrigin(playButton.getSize() / 2.0f);
    playButton.setPosition(static_cast<float>(window.getSize().x) / 2.0f,
                           static_cast<float>(window.getSize().y) / 2.0f);
    playButton.setFillColor(sf::Color::Red);

    sf::RectangleShape closeButton;
    closeButton.setSize(sf::Vector2f(200.0f, 75.0f));
    closeButton.setOrigin(closeButton.getSize() / 2.0f);
    closeButton.setPosition(static_cast<float>(window.getSize().x) / 2.0f,
                            static_cast<float>(window.getSize().y) / 2.0f + 100);
    closeButton.setFillColor(sf::Color::Yellow);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::MouseButtonPressed) {
                if (playButton.getGlobalBounds().contains(
                        static_cast<float>(sf::Mouse::getPosition(window).x),
                        static_cast<float>(sf::Mouse::getPosition(window).y))) {
                    return;
                } else if (closeButton.getGlobalBounds().contains(
                        static_cast<float>(sf::Mouse::getPosition(window).x),
                        static_cast<float>(sf::Mouse::getPosition(window).y))) {
                    window.close();
                }
            }
        }
        window.clear();
        window.draw(gameName);
        window.draw(playButton);
        window.draw(closeButton);
        window.display();
    }
}

void
finalScreen(sf::RenderWindow &window, const sf::Image &icon, const sf::Font &font, const std::string &strPlayingTime) {
    window.create(sf::VideoMode(450, 200), "Pacman Maze", sf::Style::Close);
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    sf::Text victoryText;
    textConfig(victoryText, font, "Victory!!!", 100, sf::Color::Red);
    victoryText.setPosition(static_cast<float>(window.getSize().x) / 2.0f - victoryText.getGlobalBounds().width / 2.0f,
                            0.0f);

    sf::Text timeText;
    textConfig(timeText, font, "Time: " + strPlayingTime.substr(0, strPlayingTime.find('.') + 3) + "s", 50,
               sf::Color::Green);
    timeText.setPosition(static_cast<float>(window.getSize().x) / 2.0f - timeText.getGlobalBounds().width / 2.0f,
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

#endif //PROJEKT_SCREENS_HPP
