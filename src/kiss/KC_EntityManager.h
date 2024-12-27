#pragma once

#include "KC_Entity.h"

class KC_EntityManager final
{
public:
    KC_EntityManager();

    KC_Entity CreateEntity();

private:
    KC_Entity myEntityCount;
};
