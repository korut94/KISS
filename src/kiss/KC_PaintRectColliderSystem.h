#pragma once

#include "KC_PaintSystem.h"

class KC_RectCollider;
class KC_RectColliderPalette;
class KC_Transform;

class KC_PaintRectColliderSystem final : public KC_PaintSystem<KC_Transform, KC_RectCollider, KC_RectColliderPalette>
{
    KC_SYSTEM(KC_PaintRectColliderSystem)

public:
    void Run();
};
