#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "Tile.hpp"

struct TileInfo {
    float     tileSize    = 1.0f; // World Transform Size
    float     tilePadding = 0.1f; // World Transform Padding
    sf::Color tileColor   = sf::Color::Green;
};

class Grid {
public:
    explicit Grid(sf::RenderWindow &window, unsigned int width = 0, unsigned int height = 0, TileInfo tileInfo = {});

    void render() const;

    [[nodiscard]] sf::Vector2f getGridCenterOffset() const;
    [[nodiscard]] sf::Vector2u getTileGridPosition(unsigned int tileIndex) const;
    [[nodiscard]] sf::Vector2u getTileGridPosition(sf::Vector2i screenPosition) const;
    [[nodiscard]] sf::Vector2u getTileGridPosition(sf::Vector2f worldPosition) const;
    [[nodiscard]] unsigned int getTileIndex(sf::Vector2f tileGridPosition) const;

private:
    std::vector<Tile> m_tiles;
    sf::RenderWindow &m_window;
    TileInfo m_tileInfo;
    unsigned int m_width = 0, m_height = 0;
};
