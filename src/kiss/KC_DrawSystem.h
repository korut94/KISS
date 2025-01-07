#pragma once

#include "KC_ComponentManager.h"
#include "KC_EntitySet.h"

#include <tuple>

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
