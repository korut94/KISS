#include "KC_PaintSpatialGridSystem.h"

#include "KC_Assert.h"
#include "KC_Canvas.h"
#include "KC_SpatialGridPalette.h"

void KC_PaintSpatialGridSystem::Run()
{
    for (KC_Entity entity : myEntitySet)
    {
        KC_Canvas& canvas = GetComponent<KC_Canvas>(entity);
        const KC_SpatialGridPalette& palette = GetComponent<KC_SpatialGridPalette>(entity);
        const KC_SpatialGrid& spatialGrid = palette.mySpatialGrid;

        DrawRectangle(canvas, {0.f, 0.f}, {10.f, 10.f}, sf::Color::Green);
    }
}
