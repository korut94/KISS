#pragma once

#include "KC_CollisionEvent.h"
#include "KC_ComponentManager.h"
#include "KC_EntitySet.h"
#include "KC_Profiling.h"
#include "KC_SystemProvider.h"

#include <vector>

class KC_SpatialGrid;
class KC_World;

class KC_GameSystemProvider final : public KC_SystemProvider<KC_MainComponentManager&>
{
    using Super = KC_SystemProvider<KC_MainComponentManager&>;

    template <typename TGame>
    friend class KC_GameManager;

public:
    explicit KC_GameSystemProvider(KC_World& aWorld);

    float GetElapsedTime() const;
    const std::vector<KC_SpatialGrid>& GetSpatialGrids() const;
    std::vector<KC_SpatialGrid>& GetSpatialGrids();

    template <typename TSystem, typename... Args>
    void RunSystem(Args&&... args) const;

    static float GetFixedUpdateTime() { return ourFixedUpdateTime.AsSeconds(); }

private:
    KC_World& myWorld;
    KC_Time myElapsedTime;

    static KC_Time ourFixedUpdateTime;

public:
    std::vector<KC_CollisionEvent> myCollisionEvents;
};

template <typename TSystem, typename... Args>
void KC_GameSystemProvider::RunSystem(Args&&... args) const
{
    KC_EntitySet entitySet;
    {
        KC_PROFILE(TSystem::GetEntitiesFetchTag())
        GetEntitySet<typename TSystem::Components>(entitySet);
    }
    
    TSystem system{ entitySet, myComponentManager };
    {
        KC_PROFILE(TSystem::GetRunTag())
        system.Run(std::forward<Args>(args)...);
    }
}
