#pragma once

#include <SFML/Graphics/Color.hpp>

class KC_SpatialGrid;

struct KC_SpatialGridPalette
{
    sf::Color myGridCellFillColor = sf::Color(0, 0, 255, 80);
    sf::Color myGridLineColor = sf::Color::White;
    float myGridLineThickness = 0.1f;
};
