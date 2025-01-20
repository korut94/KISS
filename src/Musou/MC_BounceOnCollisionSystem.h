#pragma once

#include "KC_GameSystem.h"

#include <vector>

class KC_CollisionEvent;
class KC_Transform;
class KC_Velocity;

class MC_BounceOnCollisionSystem final : public KC_GameSystem<KC_Velocity>
{
    KC_SYSTEM(MC_BounceOnCollisionSystem)

public:
    void Run(const std::vector<KC_CollisionEvent>& someCollisionEvents);
};
