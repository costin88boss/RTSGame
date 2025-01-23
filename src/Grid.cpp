#include "Grid.hpp"

Grid::Grid(sf::RenderWindow& window, const unsigned int width, const unsigned int height, const TileInfo tileInfo)
: m_window(window)
, m_width(width)
, m_height(height)
, m_tileInfo(tileInfo) {
    for (unsigned int i = 0; i < width * height; i++) {
        m_tiles.emplace_back(*this);
    }
}

void Grid::render() const {
    sf::RectangleShape rect;
    rect.setSize(sf::Vector2f(
        static_cast<float>(m_tileInfo.tileSize),
        static_cast<float>(m_tileInfo.tileSize)
    ));

    rect.setFillColor(m_tileInfo.tileColor);

    for (unsigned int i = 0; i < m_tiles.size(); i++) {
        rect.setPosition(sf::Vector2f(getTileGridPosition(i) * (m_tileInfo.tileSize + m_tileInfo.tilePadding)));
        m_window.draw(rect);
    }
}

sf::Vector2u Grid::getTileGridPosition(const unsigned int tileIndex) const {
    return {tileIndex % m_width, tileIndex / m_height};
}
