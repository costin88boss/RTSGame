#include "Transform.hpp"

sf::Vector2f Transform::ScreenToWorldVector(const sf::Vector2i& vector) {
    return sf::Vector2f(vector) / static_cast<float>(pixelsPerUnit);
}

sf::Vector2i Transform::WorldToScreenVector(const sf::Vector2f& vector) {
    return sf::Vector2i(vector) * static_cast<int>(pixelsPerUnit);
}
