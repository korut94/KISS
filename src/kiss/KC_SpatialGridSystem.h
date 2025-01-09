#pragma once

#include "KC_GameSystem.h"

class KC_RectCollider;
class KC_SpatialGrid;
class KC_Transform;

class KC_SpatialGridSystem final : public KC_GameSystem<KC_Transform, KC_RectCollider>
{
    KC_SYSTEM(KC_SpatialGridSystem)

public:
    void Run(KC_SpatialGrid& aSpatialGrid);
};
