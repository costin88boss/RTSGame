#pragma once

class Grid;

class Tile {
public:
    explicit Tile(Grid& grid);

private:
    Grid& m_grid;
};
