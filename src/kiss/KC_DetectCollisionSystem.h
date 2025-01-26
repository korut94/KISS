#pragma once

#include "KC_GameSystem.h"

#include <vector>

class KC_RectCollider;
class KC_Transform;
class KC_Velocity;

class KC_DetectCollisionSystem final : public KC_GameSystem<KC_Transform, KC_Velocity, KC_RectCollider>
{
    KC_SYSTEM(KC_DetectCollisionSystem)

public:
    void Run();
};
