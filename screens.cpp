#include "screens.hpp"

#include <algorithm>
#include <fstream>
#include <vector>

#include "configuration.hpp"
#include "leaderboard.hpp"

void Screens::mainMenu(sf::RenderWindow &window, const sf::Font &font) {
    sf::Text gameName;
    Configuration::textConfig(gameName, font, "Pacman Maze", 100, sf::Color::Green);
    gameName.setPosition(static_cast<float>(window.getSize().x) / 2.0f - gameName.getGlobalBounds().width / 2.0f,
                         0.0f);

    sf::RectangleShape playButton;
    playButton.setSize(sf::Vector2f(200.0f, 75.0f));
    playButton.setOrigin(playButton.getSize() / 2.0f);
    playButton.setPosition(static_cast<float>(window.getSize().x) / 2.0f,
                           static_cast<float>(window.getSize().y) / 2.0f);
    playButton.setFillColor(sf::Color::Red);

    sf::Text playButtonText;
    Configuration::textConfig(playButtonText, font, "Play", 60, sf::Color::Green);
    playButtonText.setPosition(playButton.getPosition().x - playButtonText.getGlobalBounds().width / 2,
                               playButton.getPosition().y - playButtonText.getGlobalBounds().height / 2 - 15);

    sf::RectangleShape closeButton;
    closeButton.setSize(sf::Vector2f(200.0f, 75.0f));
    closeButton.setOrigin(closeButton.getSize() / 2.0f);
    closeButton.setPosition(static_cast<float>(window.getSize().x) / 2.0f,
                            static_cast<float>(window.getSize().y) / 2.0f + 100);
    closeButton.setFillColor(sf::Color::Yellow);

    sf::Text closeButtonText;
    Configuration::textConfig(closeButtonText, font, "Exit", 60, sf::Color::Blue);
    closeButtonText.setPosition(closeButton.getPosition().x - closeButtonText.getGlobalBounds().width / 2,
                                closeButton.getPosition().y - closeButtonText.getGlobalBounds().height / 2 - 15);

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
        window.draw(playButtonText);
        window.draw(closeButton);
        window.draw(closeButtonText);
        window.display();
    }
}

void Screens::finalScreen(sf::RenderWindow &window, const sf::Font &font,
                          const std::string &strPlayingTime) {
    window.create(sf::VideoMode(450, 520), "Pacman Maze", sf::Style::Close);

    sf::Text victoryText;
    Configuration::textConfig(victoryText, font, "Win!!!", 100, sf::Color::Red);
    victoryText.setPosition(static_cast<float>(window.getSize().x) / 2.0f - victoryText.getGlobalBounds().width / 2.0f,
                            0.0f);

    sf::Text timeText;
    Configuration::textConfig(timeText, font,
                              "Your time: " + strPlayingTime.substr(0, strPlayingTime.find('.') + 3) + "s", 50,
                              sf::Color::Green);
    timeText.setPosition(static_cast<float>(window.getSize().x) / 2.0f - timeText.getGlobalBounds().width / 2.0f,
                         victoryText.getGlobalBounds().height + 15);

    Leaderboard leaderboard(strPlayingTime);
    auto topTimes = leaderboard.getBestTimes();

    std::string bestTimes = "Best times:\n";
    for (auto &time : topTimes) {
        std::string strTime = std::to_string(time);
        bestTimes += strTime.substr(0, strTime.find('.') + 3);
        bestTimes += "s\n";
    }
    sf::Text leaderBoardText;
    Configuration::textConfig(leaderBoardText, font, bestTimes, 50, sf::Color::Yellow);
    leaderBoardText.setPosition(0, victoryText.getGlobalBounds().height + 70);

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
        window.draw(leaderBoardText);
        window.display();
    }
}