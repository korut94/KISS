#pragma once

#include "KC_ComponentManager.h"
#include "KC_TemplateHelper.h"

using namespace KC_TemplateHelper;

namespace sf { class RenderWindow; }

class KC_RenderSystemProvider final
{
public:
    KC_RenderComponentManager& GetComponentManager() { return myComponentManager; }

    template <typename TService>
    void Run(sf::RenderWindow& aRenderWindow) const;

private:
    template <typename Tuple>
    void GetEntitySet(KC_EntitySet& outEntitySet) const;
    template <typename... Args>
    void GetEntitySetImpl(KC_EntitySet& outEntitySet, UnpackedTuple<Args...>) const;

    KC_RenderComponentManager myComponentManager;
};

template <typename TService>
void KC_RenderSystemProvider::Run(sf::RenderWindow& aRenderWindow) const
{
    KC_EntitySet entitySet;
    GetEntitySet<typename TService::Components>(entitySet);

    TService service { entitySet, myComponentManager };
    service.Run(aRenderWindow);
}

template <typename Tuple>
void KC_RenderSystemProvider::GetEntitySet(KC_EntitySet& outEntitySet) const
{
    GetEntitySetImpl(outEntitySet, Unpack<Tuple>{});
}

template <typename... Args>
void KC_RenderSystemProvider::GetEntitySetImpl(KC_EntitySet& outEntitySet, UnpackedTuple<Args...>) const
{
    myComponentManager.GetEntitySet<Args...>(outEntitySet);
}
