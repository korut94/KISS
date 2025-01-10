#pragma once

#include "KC_PaintSystem.h"

class KC_SpatialGrid;
class KC_SpatialGridPalette;

class KC_PaintSpatialGridSystem final : public KC_PaintSystem<KC_SpatialGridPalette>
{
    KC_SYSTEM(KC_PaintSpatialGridSystem)

public:
    void Run();
};
