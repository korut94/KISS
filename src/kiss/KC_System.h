#pragma once

#include "KC_EntitySet.h"
#include "KC_Profiling.h"

#include <tuple>

#if IS_DEBUG_BUILD
#define KC_SYSTEM_PROFILETAGS(aSystem)\
    static constexpr const char* GetEntitiesFetchTag() { return KC_PROFILE_STEP_TAG(#aSystem, "EntitiesFetch"); }\
    static constexpr const char* GetRunTag() { return KC_PROFILE_STEP_TAG(#aSystem, "Run"); }
#else
#define KC_SYSTEM_PROFILETAGS(aSystem)
#endif // IS_DEBUG_BUILD

#define KC_SYSTEM(aSystem)\
    public:\
        using KC_System::KC_System;\
        KC_SYSTEM_PROFILETAGS(aSystem)

template <typename TComponentManager, typename T, typename... Args>
class KC_System
{
public:
    using Components = std::tuple<T, Args...>;

public:
    KC_System(const KC_EntitySet& anEntitySet, TComponentManager& aComponentManager);

    template <typename G>
    const G& GetComponent(KC_Entity anEntity) const;
    template <typename G>
    G& GetComponent(KC_Entity anEntity);

protected:
    const KC_EntitySet& myEntitySet;

private:
    TComponentManager& myComponentManager;
};

template <typename TComponentManager, typename T, typename... Args>
KC_System<TComponentManager, T, Args...>::KC_System(const KC_EntitySet& anEntitySet, TComponentManager& aComponentManager)
    : myEntitySet(anEntitySet)
    , myComponentManager(aComponentManager)
{
}

template <typename TComponentManager, typename T, typename... Args>
template <typename G>
const G& KC_System<TComponentManager, T, Args...>::GetComponent(KC_Entity anEntity) const
{
    return myComponentManager.template GetComponent<G>(anEntity);
}

template <typename TComponentManager, typename T, typename... Args>
template <typename G>
G& KC_System<TComponentManager, T, Args...>::GetComponent(KC_Entity anEntity)
{
    return myComponentManager.template GetComponent<G>(anEntity);
}
