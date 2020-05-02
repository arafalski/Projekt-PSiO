#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "pacman.hpp"
#include "consts.hpp"
#include "map.hpp"
#include "collider.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Pacman Maze | FPS: 00.00");
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

    sf::Texture pacmanTexture;
    if (!pacmanTexture.loadFromFile("../Assets/Images/pacman.png")) {
        return EXIT_FAILURE;
    }
    Pacman player(&pacmanTexture, sf::Vector2u(2, 1), 0.15f, 200.0f);
    view.setCenter(player.getPosition());

    std::vector<sf::RectangleShape> obstacles = generateMap();

    sf::Clock clock;
    float deltaTime = 0;
    unsigned int count = 1;

    // game loop
    while (window.isOpen()) {
        deltaTime = clock.restart().asSeconds();

        if (count >= 60) {
            window.setTitle("Pacman Maze | FPS: " + std::to_string(1 / deltaTime));
            count = 1;
        } else {
            count++;
        }

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

        player.update(deltaTime, sf::Vector2f(window.mapPixelToCoords(sf::Mouse::getPosition(window)).x,
                                              window.mapPixelToCoords(sf::Mouse::getPosition(window)).y));
        sf::Vector2f direction;
        Collider playerCollider = player.getCollider();

        for (auto &box : obstacles) {
            if (box.getFillColor() == sf::Color::Blue && Collider(box).checkCollision(playerCollider, direction)) {
                player.onCollision(direction);
            } else if (box.getFillColor() == sf::Color::Red &&
                       Collider(box).checkCollision(playerCollider, direction)) {
                return EXIT_SUCCESS; //TODO: Add better ending scene than turning the game off :/
            } else if (box.getFillColor() == sf::Color::Yellow &&
                       Collider(box).checkCollision(playerCollider, direction)) {
                box.setFillColor(sf::Color::Black);
            }
        }

        view.setCenter(player.getPosition());
        window.clear();
        window.setView(view);
        for (auto &box : obstacles) {
            window.draw(box);
        }
        window.draw(player);
        window.display();
    }

    return 0;
}
