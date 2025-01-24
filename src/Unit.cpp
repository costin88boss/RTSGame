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

void Unit::setTargetTile(const sf::Vector2u targetTileGridPosition) {
    const sf::Vector2f centerOffset = -sf::Vector2f(1.0f ,1.0f) * (mp_grid->getTileInfo().tileSize + mp_grid->getTileInfo().tilePadding) / 2.0f;
    m_targetWorldPosition = mp_grid->getTileWorldPosition(targetTileGridPosition) + centerOffset;
}

void Unit::moveToTarget(const float deltaTime) {
    if (m_targetWorldPosition == m_worldPosition) return;
    move(sf::Vector2f(m_targetWorldPosition - m_worldPosition).normalized() * deltaTime);
}
