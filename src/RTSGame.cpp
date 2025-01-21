#include <SFML/Graphics.hpp>

int main() {
  auto window = sf::RenderWindow(sf::VideoMode({720u, 480u}), "Hello, SFML!");

  while (window.isOpen()) {
    while (const std::optional event = window.pollEvent()) {
      if (!event->is<sf::Event::Closed>()) break;
      window.close();
    }

    window.clear();
    window.display();
  }
}
