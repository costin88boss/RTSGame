#include <iostream>
#include <SFML/Graphics.hpp>
#include "Grid.hpp"
#include "Transform.hpp"

namespace netTest {
    extern void testMain();
}

int main() {
    auto window = sf::RenderWindow(sf::VideoMode({720, 480}), "Hello, SFML!");
    // netTest::testMain();
    const Grid grid(window, 16, 16);
    Transform::pixelsPerUnit = 25u;

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();
        grid.render();
        window.display();

        std::cout << "You are touching tile " <<
            grid.getTileGridPosition(sf::Mouse::getPosition(window)).x << ", " <<
            grid.getTileGridPosition(sf::Mouse::getPosition(window)).y << '\n';
    }
}
