#include "Grid.hpp"

#include "Transform.hpp"
#include "Unit.hpp"

Grid::Grid(sf::RenderWindow& window, const unsigned int width, const unsigned int height, const TileInfo tileInfo)
: m_window(window)
, m_tileInfo(tileInfo)
, m_width(width)
, m_height(height) {
    for (unsigned int i = 0; i < width * height; i++) {
        m_tiles.emplace_back(*this);
    }
}

void Grid::render() const {
    const float tileScreenSize    = m_tileInfo.tileSize    * Transform::pixelsPerUnit;
    const float tileScreenPadding = m_tileInfo.tilePadding * Transform::pixelsPerUnit;

    sf::RectangleShape rect;
    rect.setSize({tileScreenSize, tileScreenSize});
    rect.setFillColor(m_tileInfo.tileColor);

    for (unsigned int i = 0; i < m_tiles.size(); i++) {
        rect.setPosition(
            sf::Vector2f(getTileGridPosition(i)) * (tileScreenSize + tileScreenPadding) + getGridCenterOffset());
        m_window.draw(rect);
    }

    rect.setFillColor(sf::Color::Cyan);

    for (const auto& unit : m_units) {
        rect.setPosition(
            sf::Vector2f(unit.m_worldPosition) * (tileScreenSize + tileScreenPadding) + getGridCenterOffset());
        m_window.draw(rect);
    }
}

sf::Vector2f Grid::getGridCenterOffset() const {
    const float tileScreenSize    = m_tileInfo.tileSize    * Transform::pixelsPerUnit;
    const float tileScreenPadding = m_tileInfo.tilePadding * Transform::pixelsPerUnit;

    const sf::Vector2f windowSize = sf::Vector2f(m_window.getSize());
    const sf::Vector2f gridScreenSize = {
        m_width * (tileScreenSize + tileScreenPadding), m_height * (tileScreenSize + tileScreenPadding)};

    return (windowSize - gridScreenSize) / 2.0f;
}

sf::Vector2u Grid::getTileGridPosition(const unsigned int tileIndex) const {
    return {tileIndex % m_width, tileIndex / m_width};
}

sf::Vector2u Grid::getTileGridPosition(const sf::Vector2i screenPosition) const {
    const sf::Vector2f worldPosition = Transform::ScreenToWorldVector(screenPosition - sf::Vector2i(getGridCenterOffset()));
    return sf::Vector2u(worldPosition / (m_tileInfo.tileSize + m_tileInfo.tilePadding));
}

sf::Vector2u Grid::getTileGridPosition(const sf::Vector2f worldPosition) const {
    return sf::Vector2u(worldPosition / (m_tileInfo.tileSize + m_tileInfo.tilePadding));
}

unsigned int Grid::getTileIndex(const sf::Vector2u tileGridPosition) const {
    return tileGridPosition.y * m_width + tileGridPosition.x;
}

Unit& Grid::createUnit(const sf::Vector2u tileGridPosition) {
    unsigned int index = 0;
    while (containsUnitWithIndex(index)) {
        index++;
    }

    return m_units.emplace_back(index, std::make_shared<Grid>(*this), sf::Vector2f(tileGridPosition));
}

void Grid::deleteUnit(const unsigned int unitIndex) {
    std::erase_if(m_units, [&](const Unit& unit) { return unit.m_unitIndex == unitIndex; });
}

bool Grid::containsUnitWithIndex(const unsigned int index) const {
    for (const auto& unit : m_units) {
        if (unit.m_unitIndex == index) {
            return true;
        }
    }

    return false;
}
