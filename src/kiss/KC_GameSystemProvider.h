#pragma once

#include "KC_ComponentManager.h"
#include "KC_EntitySet.h"
#include "KC_Profiling.h"
#include "KC_TemplateHelper.h"

using namespace KC_TemplateHelper;

class KC_GameSystemProvider final
{
public:
    explicit KC_GameSystemProvider(KC_MainComponentManager& aComponentManager);

    template <typename TSystem>
    void RunSystem() const;

private:
    template <typename Tuple>
    void GetEntitySet(KC_EntitySet& outEntitySet) const;
    template <typename... Args>
    void GetEntitySetImpl(KC_EntitySet& outEntitySet, UnpackedTuple<Args...>) const;

    KC_MainComponentManager& myComponentManager;
};

template <typename TSystem>
void KC_GameSystemProvider::RunSystem() const
{
    KC_EntitySet entitySet;
    {
        KC_PROFILE(TSystem::GetEntitiesFetchTag())
        GetEntitySet<typename TSystem::Components>(entitySet);
    }
    
    TSystem system{ entitySet, myComponentManager };
    {
        KC_PROFILE(TSystem::GetRunTag())
        system.Run();
    }
}

template <typename Tuple>
void KC_GameSystemProvider::GetEntitySet(KC_EntitySet &outEntitySet) const
{
    GetEntitySetImpl(outEntitySet, Unpack<Tuple>{});
}

template <typename... Args>
void KC_GameSystemProvider::GetEntitySetImpl(KC_EntitySet &outEntitySet, UnpackedTuple<Args...>) const
{
    myComponentManager.GetEntitySet<Args...>(outEntitySet);
}
