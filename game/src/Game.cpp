#include "Game.h"

#include <SFML/Graphics.hpp>

Game::Game() {
    auto window = sf::RenderWindow(sf::VideoMode({720, 480}), "Hello, SFML!");

    sf::Clock clock;
    float deltaTime = 0.0f;

    const sf::Font font("../../res/fonts/Roboto-Regular.ttf");
    sf::Text text(font);
    text.setFillColor(sf::Color::Green);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();

        window.draw(text);
        window.display();

        deltaTime = clock.getElapsedTime().asSeconds();
        text.setString(std::to_string(static_cast<int>(1.0f / deltaTime)));

        clock.restart();
    }
}

Game::~Game() {
    
}
