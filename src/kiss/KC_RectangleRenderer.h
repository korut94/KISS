#pragma once

#include "KC_ShapeRenderer.h"

#include <SFML/System/Vector2.hpp>

struct KC_RectangleRenderer : public KC_ShapeRenderer
{
    sf::Vector2f mySize = sf::Vector2f(1.f, 1.f);
};
