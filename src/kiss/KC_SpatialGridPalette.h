#pragma once

class KC_SpatialGrid;

struct KC_SpatialGridPalette
{
    KC_SpatialGridPalette(const KC_SpatialGrid& aSpatialGrid) : mySpatialGrid(aSpatialGrid) {}

    const KC_SpatialGrid& mySpatialGrid;
};
