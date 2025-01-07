#pragma once

#include "KC_ComponentManager.h"
#include "KC_EntitySet.h"
#include "KC_Profiling.h"

#include <tuple>

#if IS_DEBUG_BUILD
#define KC_DRAWSYSTEM_PROFILETAGS(aSystem)\
    static constexpr const char* GetEntitiesFetchTag() { return KC_PROFILE_STEP_TAG(#aSystem, "EntitesFetch"); }\
    static constexpr const char* GetRunTag() { return KC_PROFILE_STEP_TAG(#aSystem, "Run"); }
#else
#define KC_DRAWSYSTEM_PROFILETAGS(aSystem)
#endif // IS_DEBUG_BUILD

#define KC_DRAWSYSTEM(aSystem)\
    public:\
        using KC_DrawSystem::KC_DrawSystem;\
        KC_DRAWSYSTEM_PROFILETAGS(aSystem)

namespace sf { class RenderWindow; }

template <typename T, typename... Args>
class KC_DrawSystem
{
public:
    using Components = std::tuple<T, Args...>;

public:
    KC_DrawSystem(const KC_EntitySet& anEntitySet, const KC_RenderComponentManager& aComponentManager);

    template <typename G>
    const G& GetComponent(KC_Entity anEntity) const;

protected:
    const KC_EntitySet& myEntitySet;

private:
    const KC_RenderComponentManager& myComponentManager;
};

template <typename T, typename... Args>
KC_DrawSystem<T, Args...>::KC_DrawSystem(const KC_EntitySet& anEntitySet, const KC_RenderComponentManager& aComponentManager)
    : myEntitySet(anEntitySet)
    , myComponentManager(aComponentManager)
{
}

template <typename T, typename... Args>
template <typename G>
const G& KC_DrawSystem<T, Args...>::GetComponent(KC_Entity anEntity) const
{
    return myComponentManager.GetComponent<G>(anEntity);
}
