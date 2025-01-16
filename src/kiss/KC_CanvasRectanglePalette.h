#pragma once

#include <SFML/Graphics/Color.hpp>

struct KC_CanvasRectanglePalette
{
    sf::Color myFillColor = sf::Color::White;
    sf::Color myOutlineColor = sf::Color::Black;
    float myOutlineThickness = 0.1f;
};
