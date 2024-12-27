#pragma once

#include "KC_Assert.h"
#include "KC_ComponentArray.h"
#include "KC_ComponentsRegistry.h"
#include "KC_Entity.h"
#include "KC_TemplateHelper.h"

#include <array>
#include <memory>

template <typename TComponents>
class KC_ComponentManager final
{
    static constexpr const std::size_t ComponentsCount = std::tuple_size<TComponents>::value;

public:
    KC_ComponentManager();

    template <typename TOtherComponents>
    void AssignComponents(KC_ComponentManager<TOtherComponents>& anOther) const;
    template <typename T>
    T& GetComponent(KC_Entity anEntity);
    template <typename T>
    const T& GetComponent(KC_Entity anEntity) const;
    template <typename T>
    const KC_ComponentArrayImpl<T>& GetComponentArray() const;
    template <typename T>
    KC_ComponentArrayImpl<T>& GetComponentArray();
    template <typename T, typename... OtherArgs>
    void GetEntitySet(KC_EntitySet& outEntitySet) const;

    template <typename T>
    T& AddComponent(KC_Entity anEntity);

private:
    template <typename... Args>
    void AssignComponentsImpl(KC_ComponentManager<std::tuple<Args...>>& anOther) const;

    template <std::size_t Index, std::size_t Count>
    inline void RegisterComponents();
    template <std::size_t Index, typename T>
    void RegisterComponent();

    std::array<std::unique_ptr<KC_ComponentArray>, ComponentsCount> myComponentArrays;
};

template <typename TComponents>
KC_ComponentManager<TComponents>::KC_ComponentManager()
{
    RegisterComponents<0, ComponentsCount>();
}

template <typename TComponents>
template <typename TOtherComponents>
void KC_ComponentManager<TComponents>::AssignComponents(KC_ComponentManager<TOtherComponents>& anOther) const
{
    AssignComponentsImpl(anOther);
}

template <typename TComponents>
template <typename T>
T& KC_ComponentManager<TComponents>::GetComponent(KC_Entity anEntity)
{
    return GetComponentArray<T>().GetComponent(anEntity);
}

template <typename TComponents>
template <typename T>
const T& KC_ComponentManager<TComponents>::GetComponent(KC_Entity anEntity) const
{
    return GetComponentArray<T>().GetComponent(anEntity);
}

template <typename TComponents>
template <typename T>
T& KC_ComponentManager<TComponents>::AddComponent(KC_Entity anEntity)
{
    return GetComponentArray<T>().InsertComponent(anEntity);
}

template <typename TComponents>
template <typename T>
const KC_ComponentArrayImpl<T>& KC_ComponentManager<TComponents>::GetComponentArray() const
{
    static constexpr const std::size_t index = KC_TemplateHelper::GetTupleTypeIndex<T, TComponents>::value;
    KC_STATIC_ASSERT(index < ComponentsCount, "Component not found: did you forget to register it?");

    return static_cast<const KC_ComponentArrayImpl<T>&>(*myComponentArrays[index]);
}

template <typename TComponents>
template <typename T>
KC_ComponentArrayImpl<T>& KC_ComponentManager<TComponents>::GetComponentArray()
{
    static constexpr const std::size_t index = KC_TemplateHelper::GetTupleTypeIndex<T, TComponents>::value;
    KC_STATIC_ASSERT(index < ComponentsCount, "Component not found: did you forget to register it?");

    return static_cast<KC_ComponentArrayImpl<T>&>(*myComponentArrays[index]);
}

template <typename TComponents>
template <typename T, typename... OtherArgs>
void KC_ComponentManager<TComponents>::GetEntitySet(KC_EntitySet& outEntitySet) const
{
    outEntitySet = GetComponentArray<T>().GetEntitySet();
    ((outEntitySet = std::move(outEntitySet.Intersect(GetComponentArray<OtherArgs>().GetEntitySet()))), ...);
}

template <typename TComponents>
template <typename... Args>
void KC_ComponentManager<TComponents>::AssignComponentsImpl(KC_ComponentManager<std::tuple<Args...>>& anOther) const
{
    ((anOther.template GetComponentArray<Args>() = GetComponentArray<Args>()), ...);
}

template <typename TComponents>
template <std::size_t Index, std::size_t Count>
void KC_ComponentManager<TComponents>::RegisterComponents()
{
    using ComponentType = typename std::tuple_element<Index, TComponents>::type;

    RegisterComponent<Index, ComponentType>();

    if constexpr (Index + 1 < Count)
    {
        RegisterComponents<Index + 1, Count>();
    }
}

template <typename TComponents>
template <std::size_t Index, typename T>
void KC_ComponentManager<TComponents>::RegisterComponent()
{
    KC_ASSERT(myComponentArrays[Index] == nullptr, "Component already registered");
    myComponentArrays[Index] = std::move(std::make_unique<KC_ComponentArrayImpl<T>>());
}

using KC_MainComponentManager = KC_ComponentManager<KC_ComponentsRegistry::MainComponents>;
using KC_RenderComponentManager = KC_ComponentManager<KC_ComponentsRegistry::RenderComponents>;
