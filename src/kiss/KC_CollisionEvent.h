#pragma once

#include "KC_Entity.h"

struct KC_CollisionEvent
{
    KC_CollisionEvent(KC_Entity anEntity, float aContactTime)
        : myHitEntity(anEntity)
        , myContactTime(aContactTime)
    {}

    KC_Entity myHitEntity;
    float myContactTime;
};
