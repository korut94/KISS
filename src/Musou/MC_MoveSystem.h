#pragma once

#include "KC_GameSystem.h"

class KC_Transform;
class KC_Velocity;

class MC_MoveSystem final : public KC_GameSystem<KC_Transform, KC_Velocity>
{
    KC_SYSTEM(MC_MoveSystem)

public:
    void Run();
};
