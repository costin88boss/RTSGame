#pragma once

#include <SFML/System.hpp>
#include "SFML/Window.hpp"

namespace Transform {
    inline unsigned int pixelsPerUnit = 25u;

    sf::Vector2f ScreenToWorldVector(const sf::Vector2i& vector);
    sf::Vector2i WorldToScreenVector(const sf::Vector2f& vector);
};  // namespace Transform
