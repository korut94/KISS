#pragma once

#include "KC_GameSystem.h"

#include <vector>

class KC_CollisionEvent; 
class KC_RectCollider;
class KC_SpatialGrid;
class KC_Transform;

class KC_ResolveCollisionSystem final : public KC_GameSystem<KC_Transform, KC_RectCollider>
{
    KC_SYSTEM(KC_ResolveCollisionSystem)

public:
    void Run();
};
