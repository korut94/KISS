#include "KC_World.h"

KC_World::KC_World()
{
    mySpatialGrids.reserve(4);
}

KC_SpatialGrid& KC_World::AddSpatialGrid(float aGridCellSize)
{
    return mySpatialGrids.emplace_back(aGridCellSize);
}
