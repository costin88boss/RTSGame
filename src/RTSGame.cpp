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

    Grid grid(window, 16, 16);
    auto& unit = grid.createUnit({0, 0});

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        if (isKeyPressed(sf::Keyboard::Key::Right)) {
            unit.move({0.01f, 0.0f});
        }
        if (isKeyPressed(sf::Keyboard::Key::Left)) {
            unit.move({-0.01f, 0.0f});
        }
        if (isKeyPressed(sf::Keyboard::Key::Down)) {
            unit.move({0.0f, 0.01f});
        }
        if (isKeyPressed(sf::Keyboard::Key::Up)) {
            unit.move({0.0f, -0.01f});
        }

        window.clear();
        grid.render();
        window.display();
    }
}
