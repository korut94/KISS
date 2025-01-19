#pragma once

#include "KC_ComponentManager.h"
#include "KC_EntityManager.h"
#include "KC_SpatialGrid.h"

#include <vector>

class KC_World final
{
public:
    KC_World();

    const KC_MainComponentManager& GetComponentManager() const { return myComponentManager; };
    KC_MainComponentManager& GetComponentManager() { return myComponentManager; }
    
    const KC_EntityManager& GetEntityManager() const { return myEntityManager; }
    KC_EntityManager& GetEntityManager() { return myEntityManager; }

    const std::vector<KC_SpatialGrid>& GetSpatialGrids() const { return mySpatialGrids; }
    std::vector<KC_SpatialGrid>& GetSpatialGrids() { return mySpatialGrids; }
    KC_SpatialGrid& AddSpatialGrid(float aGridCellSize);

private:
    KC_MainComponentManager myComponentManager;
    KC_EntityManager myEntityManager;
    std::vector<KC_SpatialGrid> mySpatialGrids;
};
