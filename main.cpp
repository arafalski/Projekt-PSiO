#include <SFML/Graphics.hpp>
#include "pacman.hpp"

constexpr unsigned short WINDOW_HEIGHT = 600;
constexpr unsigned short WINDOW_WIDTH = WINDOW_HEIGHT;

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Pacman");
    sf::Image icon;
    if (!icon.loadFromFile("../Assets/Images/icon.png")) {
        return EXIT_FAILURE;
    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);

    sf::View view = window.getDefaultView();
    window.setView(view);

    sf::Mouse::setPosition(sf::Vector2i(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), window);

    Pacman player(100000.0f);

    sf::Clock clock;
    float deltaTime = 0;

    // game loop
    while (window.isOpen()) {
        deltaTime = clock.restart().asSeconds();

        // event loop
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::Resized) {
                sf::FloatRect visibleArea(0.f, 0.f, static_cast<float>(event.size.width),
                                          static_cast<float>(event.size.height));
                window.setView(sf::View(visibleArea));
            }
        }

//        player.update(deltaTime, sf::Vector2f(static_cast<float>(sf::Mouse::getPosition(window).x),
//                                              static_cast<float>(sf::Mouse::getPosition(window).y)));
        player.update(deltaTime, sf::Vector2f(window.mapPixelToCoords(sf::Mouse::getPosition(window)).x,
                                              window.mapPixelToCoords(sf::Mouse::getPosition(window)).y));

        window.clear();
        player.draw(window);
        window.display();
    }

    return 0;
}
