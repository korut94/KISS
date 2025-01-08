#pragma once

#include "KC_ComponentManager.h"
#include "KC_EntitySet.h"
#include "KC_Profiling.h"
#include "KC_SystemProvider.h"

class KC_GameSystemProvider final : public KC_SystemProvider<KC_MainComponentManager&>
{
    using Super = KC_SystemProvider<KC_MainComponentManager&>;

public:
    explicit KC_GameSystemProvider(KC_MainComponentManager& aComponentManager);

    template <typename TSystem>
    void RunSystem(float anElapsedTime) const;
};

template <typename TSystem>
void KC_GameSystemProvider::RunSystem(float anElapsedTime) const
{
    KC_EntitySet entitySet;
    {
        KC_PROFILE(TSystem::GetEntitiesFetchTag())
        GetEntitySet<typename TSystem::Components>(entitySet);
    }
    
    TSystem system{ entitySet, myComponentManager };
    {
        KC_PROFILE(TSystem::GetRunTag())
        system.Run(anElapsedTime);
    }
}
