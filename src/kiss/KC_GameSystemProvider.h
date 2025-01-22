#pragma once

#include "KC_CollisionEvent.h"
#include "KC_ComponentManager.h"
#include "KC_EntitySet.h"
#include "KC_Profiling.h"
#include "KC_SystemProvider.h"
#include "KC_World.h"

#include <vector>

class KC_SpatialGrid;

class KC_GameSystemProvider final : public KC_SystemProvider<KC_MainComponentManager&>
{
    using Super = KC_SystemProvider<KC_MainComponentManager&>;

    template <typename TGame>
    friend class KC_GameManager;
    template <typename T, typename... Args>
    friend class KC_GameSystem;

public:
    explicit KC_GameSystemProvider(KC_World& aWorld);

    float GetElapsedTime() const { return myElapsedTime.AsSeconds(); }

    const std::vector<KC_CollisionEvent>& GetCollisionEvents() const { return myCollisionEvents; };
    std::vector<KC_CollisionEvent>& GetCollisionEvents() { return myCollisionEvents; };

    const KC_SpatialGrid& GetSpatialGrid(size_t anIndex) const { return myWorld.GetSpatialGrid(anIndex); }
    KC_SpatialGrid& GetSpatialGrid(size_t anIndex) { return myWorld.GetSpatialGrid(anIndex); }
    const std::vector<KC_SpatialGrid>& GetSpatialGrids() const { return myWorld.GetSpatialGrids(); }
    std::vector<KC_SpatialGrid>& GetSpatialGrids() { return myWorld.GetSpatialGrids(); }

    template <typename TSystem, typename... Args>
    void RunSystem(Args&&... args);

    static float GetFixedUpdateTime() { return ourFixedUpdateTime.AsSeconds(); }

private:
    KC_World& myWorld;
    KC_Time myElapsedTime;
    std::vector<KC_CollisionEvent> myCollisionEvents;

    static KC_Time ourFixedUpdateTime;
};

template <typename TSystem, typename... Args>
void KC_GameSystemProvider::RunSystem(Args&&... args)
{
    KC_EntitySet entitySet;
    {
        KC_PROFILE(TSystem::GetEntitiesFetchTag())
        GetEntitySet<typename TSystem::Components>(entitySet);
    }
    
    TSystem system{ entitySet, *this };
    {
        KC_PROFILE(TSystem::GetRunTag())
        system.Run(std::forward<Args>(args)...);
    }
}
