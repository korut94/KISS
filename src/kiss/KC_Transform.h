#pragma once

#include <SFML/System/Vector2.hpp>

struct KC_Transform
{
    sf::Vector2f myPosition = sf::Vector2f(0.f, 0.f);
    sf::Vector2f myScale = sf::Vector2f(1.f, 1.f);
    float myRotation = 0.f;
};
