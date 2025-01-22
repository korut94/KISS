#pragma once

#include "KC_GameSystem.h"

#include <vector>

class KC_RectCollider;
class KC_SpatialGrid;
class KC_Transform;

class KC_InitializeSpatialGridSystem final : public KC_GameSystem<KC_Transform, KC_RectCollider>
{
    KC_SYSTEM(KC_InitializeSpatialGridSystem)

public:
    void Run();
};
