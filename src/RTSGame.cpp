#include <iostream>
#include <SFML/Graphics.hpp>
#include "Grid.hpp"
#include "Transform.hpp"

namespace netTest {
    extern void testMain();
}

int main() {
    // netTest::testMain();

    auto window = sf::RenderWindow(sf::VideoMode({720, 480}), "Hello, SFML!");

    Grid grid(window, 24, 16);
    auto& unit = grid.createUnit({0, 0});
    unit.setTargetTile({5u, 5u});

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

        unit.moveToTarget(deltaTime);

        window.clear();
        grid.render();
        window.draw(text);
        window.display();

        deltaTime = clock.getElapsedTime().asSeconds();
        text.setString(std::to_string(static_cast<int>(1.0f / deltaTime)));

        clock.restart();
    }
}
