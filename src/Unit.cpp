#include "Unit.hpp"
#include "Grid.hpp"

Unit::Unit(const unsigned int index, const std::shared_ptr<Grid>& p_grid, const sf::Vector2f worldPosition)
: m_unitIndex(index)
, mp_grid(p_grid)
, m_worldPosition(worldPosition) {
}

void Unit::move(const sf::Vector2f& translation) {
    m_worldPosition += translation;
}
