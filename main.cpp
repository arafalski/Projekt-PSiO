#include <SFML/Graphics.hpp>
#include "pacman.hpp"
#include <iostream>

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

    sf::Texture pacmanTexture;
    if(!pacmanTexture.loadFromFile("../Assets/Images/pacman.png")){
        return EXIT_FAILURE;
    }
    Pacman player(&pacmanTexture, 50000.0f);

    sf::Clock clock;
    float deltaTime = 0;
    int count = 1; //TODO: DELETE - USED ONLY BY TEMP FPS COUNTER

    // game loop
    while (window.isOpen()) {
        deltaTime = clock.restart().asSeconds();

        //TODO: DELETE - TEMP FPS COUNTER
        if(count >= 60){
            std::cout << "FPS: " << 1 / deltaTime << '\n';
            count = 1;
        }
        else{
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

//        player.update(deltaTime, sf::Vector2f(static_cast<float>(sf::Mouse::getPosition(window).x),
//                                              static_cast<float>(sf::Mouse::getPosition(window).y)));
        player.update(deltaTime, sf::Vector2f(window.mapPixelToCoords(sf::Mouse::getPosition(window)).x,
                                              window.mapPixelToCoords(sf::Mouse::getPosition(window)).y));

        window.clear();
        window.draw(player);
        window.display();
    }

    return 0;
}
