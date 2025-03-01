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
    const std::vector<KC_CollisionEvent>& GetCollisionEvents(KC_Entity anEntity) const { return myGameSystemProvider.myCollisionEventSet.GetCollisionEvents(anEntity); }
    float GetElapsedTime() const { return myGameSystemProvider.GetElapsedTime(); };
    bool HasCollision(KC_Entity anEntity) const { return myGameSystemProvider.myCollisionEventSet.HasCollision(anEntity); }

    const KC_CollisionEventSet& GetCollisionEventSet() const { return myGameSystemProvider.myCollisionEventSet; };
    KC_CollisionEventSet& GetCollisionEventSet() { return myGameSystemProvider.myCollisionEventSet; };

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
