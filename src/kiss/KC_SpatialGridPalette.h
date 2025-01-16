#pragma once

#include <SFML/Graphics/Color.hpp>

class KC_SpatialGrid;

struct KC_SpatialGridPalette
{
    KC_SpatialGridPalette(const KC_SpatialGrid& aSpatialGrid) : mySpatialGrid(aSpatialGrid) {}

    const KC_SpatialGrid& mySpatialGrid;
    sf::Color myGridCellFillColor = sf::Color(0, 0, 255, 80);
    sf::Color myGridLineColor = sf::Color::White;
    float myGridLineThickness = 0.1f;
};
