#pragma once

#include "KC_Rect.h"

namespace KC_Intersection
{
bool RectVsRect(const KC_FloatRect& a, const KC_FloatRect& b);
bool MovingRectVsMovingRect(
    const KC_FloatRect& aRectA, const KC_FloatRect& aRectB,
    sf::Vector2f aVelocityA, sf::Vector2f aVelocityB,
    float& outContactTime
);
} // namespace KC_Intersection
