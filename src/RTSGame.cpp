#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

namespace netTest {
    extern void testMain();
}

int main() {
   // auto window = sf::RenderWindow(sf::VideoMode({720, 480}), "Hello, SFML!");
    
    netTest::testMain(); /*

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (!event->is<sf::Event::Closed>())
                break;
            window.close();
        }

        window.clear();



        window.display();
    }*/
}
