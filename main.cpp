#include <SFML/Graphics.hpp>
#include "pacman.hpp"

constexpr unsigned short WINDOW_HEIGHT = 600;
constexpr unsigned short WINDOW_WIDTH = WINDOW_HEIGHT;

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Pacman");
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);
    sf::Mouse::setPosition(sf::Vector2i(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), window);

    Pacman player(10000.0f);

    sf::Clock clock;
    float deltaTime = 0;

    while (window.isOpen()) {
        deltaTime = clock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        player.update(deltaTime, sf::Vector2f(static_cast<float>(sf::Mouse::getPosition(window).x),
                                              static_cast<float>(sf::Mouse::getPosition(window).y)));

        window.clear();
        player.draw(window);
        window.display();
    }

    return 0;
}
