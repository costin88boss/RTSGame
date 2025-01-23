#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "Tile.hpp"

struct TileInfo {
    unsigned int tileSize    = 25u;
    unsigned int tilePadding = 5u;
    sf::Color    tileColor   = sf::Color::Green;
};

class Grid {
public:
    explicit Grid(sf::RenderWindow &window, unsigned int width = 0, unsigned int height = 0, TileInfo tileInfo = {});

    void render() const;

private:
    [[nodiscard]] sf::Vector2u getTileGridPosition(unsigned int tileIndex) const;

    std::vector<Tile> m_tiles;
    sf::RenderWindow &m_window;
    TileInfo m_tileInfo;
    unsigned int m_width = 0, m_height = 0;
};
