#pragma once

#include "KC_ComponentManager.h"
#include "KC_GameSystemProvider.h"
#include "KC_System.h"

template <typename T, typename... Args>
class KC_GameSystem : public KC_System<KC_MainComponentManager, T, Args...>
{
    KC_DERIVED_SYSTEM(KC_GameSystem)
    using Super = KC_System<KC_MainComponentManager, T, Args...>;

public:
    KC_GameSystem(const KC_EntitySet& anEntitySet, KC_GameSystemProvider& aGameSystemProvider);

protected:
    float GetElapsedTime() const { return myGameSystemProvider.GetElapsedTime(); };

    const std::vector<KC_CollisionEvent>& GetCollisionEvents() const { return myGameSystemProvider.GetCollisionEvents(); };
    std::vector<KC_CollisionEvent>& GetCollisionEvents() { return myGameSystemProvider.GetCollisionEvents(); };

    const KC_SpatialGrid& GetSpatialGrid(size_t anIndex) const { return myGameSystemProvider.GetSpatialGrid(anIndex); }
    KC_SpatialGrid& GetSpatialGrid(size_t anIndex) { return myGameSystemProvider.GetSpatialGrid(anIndex); }
    const std::vector<KC_SpatialGrid>& GetSpatialGrids() const { return myGameSystemProvider.GetSpatialGrids(); };
    std::vector<KC_SpatialGrid>& GetSpatialGrids() { return myGameSystemProvider.GetSpatialGrids(); };

    static float GetFixedUpdateTime() { return KC_GameSystemProvider::GetFixedUpdateTime(); };

private:
    KC_GameSystemProvider& myGameSystemProvider;
};

template <typename T, typename... Args>
KC_GameSystem<T, Args...>::KC_GameSystem(const KC_EntitySet& anEntitySet, KC_GameSystemProvider& aGameSystemProvider)
    : Super(anEntitySet, aGameSystemProvider.myComponentManager)
    , myGameSystemProvider(aGameSystemProvider)
{
}
