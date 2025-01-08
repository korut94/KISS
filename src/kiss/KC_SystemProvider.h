#pragma once

#include "KC_TemplateHelper.h"

using namespace KC_TemplateHelper;

class KC_EntitySet;

template <typename TComponentManager>
class KC_SystemProvider
{
protected:
    KC_SystemProvider() = default;

    template <typename = std::enable_if<std::is_reference_v<TComponentManager>>>
    KC_SystemProvider(TComponentManager aComponentManager) : myComponentManager(aComponentManager) {}

    template <typename Tuple>
    void GetEntitySet(KC_EntitySet& outEntitySet) const;

private:
    template <typename... Args>
    void GetEntitySetImpl(KC_EntitySet& outEntitySet, UnpackedTuple<Args...>) const;

protected:
    TComponentManager myComponentManager;
};

template <typename TComponentManager>
template <typename Tuple>
void KC_SystemProvider<TComponentManager>::GetEntitySet(KC_EntitySet& outEntitySet) const
{
    GetEntitySetImpl(outEntitySet, Unpack<Tuple>{});
}

template <typename TComponentManager>
template <typename... Args>
void KC_SystemProvider<TComponentManager>::GetEntitySetImpl(KC_EntitySet& outEntitySet, UnpackedTuple<Args...>) const
{
    myComponentManager.template GetEntitySet<Args...>(outEntitySet);
}
