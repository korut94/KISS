#pragma once

#include "KC_GameSystem.h"

class KC_Canvas;

class KC_ClearCanvasSystem final : public KC_GameSystem<KC_Canvas>
{
    KC_SYSTEM(KC_ClearCanvasSystem)

public:
    void Run();
};
