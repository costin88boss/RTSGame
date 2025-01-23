#pragma once

#include "SFML/System.hpp"

class Grid;

class Unit {
public:
    friend class Grid;
    explicit Unit(unsigned int index, const std::shared_ptr<Grid>& p_grid, sf::Vector2f worldPosition = {0.0f, 0.0f});

    void move(const sf::Vector2f& translation);

private:
    unsigned int m_unitIndex;
    std::shared_ptr<Grid> mp_grid;
    sf::Vector2f m_worldPosition;
};
