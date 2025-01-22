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
    static constexpr const size_t ComponentsCount = std::tuple_size<TComponents>::value;

public:
    KC_ComponentManager();
    ~KC_ComponentManager();

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

    template <typename T, typename... Args>
    T& AddComponent(KC_Entity anEntity, Args&&... args);

private:
    template <typename... Args>
    void AssignComponentsImpl(KC_ComponentManager<std::tuple<Args...>>& anOther) const;

    template <size_t... Index>
    inline void RegisterComponents(std::index_sequence<Index...>);
    template <size_t Index>
    void RegisterComponent();
    template <size_t... Index>
    inline void DeleteComponents(std::index_sequence<Index...>);
    template <size_t Index>
    void DeleteComponent();

    std::array<KC_ComponentArray*, ComponentsCount> myComponentArrays;
};

template <typename TComponents>
KC_ComponentManager<TComponents>::KC_ComponentManager()
{
    myComponentArrays.fill(nullptr);
    RegisterComponents(std::make_index_sequence<ComponentsCount>());
}

template <typename TComponents>
KC_ComponentManager<TComponents>::~KC_ComponentManager()
{
    DeleteComponents(std::make_index_sequence<ComponentsCount>());
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
template <typename T, typename... Args>
T& KC_ComponentManager<TComponents>::AddComponent(KC_Entity anEntity, Args&&... args)
{
    return GetComponentArray<T>().InsertComponent(anEntity, std::forward<Args>(args)...);
}

template <typename TComponents>
template <typename T>
const KC_ComponentArrayImpl<T>& KC_ComponentManager<TComponents>::GetComponentArray() const
{
    static constexpr const size_t index = KC_TemplateHelper::GetTupleTypeIndex<T, TComponents>::value;
    KC_STATIC_ASSERT(index < ComponentsCount, "Component not found: did you forget to register it?");

    return static_cast<const KC_ComponentArrayImpl<T>&>(*myComponentArrays[index]);
}

template <typename TComponents>
template <typename T>
KC_ComponentArrayImpl<T>& KC_ComponentManager<TComponents>::GetComponentArray()
{
    static constexpr const size_t index = KC_TemplateHelper::GetTupleTypeIndex<T, TComponents>::value;
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
template <size_t... Index>
inline void KC_ComponentManager<TComponents>::RegisterComponents(std::index_sequence<Index...>)
{
    ((RegisterComponent<Index>()), ...);
}

template <typename TComponents>
template <size_t Index>
void KC_ComponentManager<TComponents>::RegisterComponent()
{
    using ComponentType = typename std::tuple_element<Index, TComponents>::type;

    KC_ASSERT(myComponentArrays[Index] == nullptr, "Component already registered");
    myComponentArrays[Index] = new KC_ComponentArrayImpl<ComponentType>();
}

template <typename TComponents>
template <size_t... Index>
inline void KC_ComponentManager<TComponents>::DeleteComponents(std::index_sequence<Index...>)
{
    ((DeleteComponent<Index>()), ...);
}

template <typename TComponents>
template <size_t Index>
void KC_ComponentManager<TComponents>::DeleteComponent()
{
    using ComponentType = typename std::tuple_element<Index, TComponents>::type;

    delete static_cast<KC_ComponentArrayImpl<ComponentType>*>(myComponentArrays[Index]);
    myComponentArrays[Index] = nullptr;
}

using KC_MainComponentManager = KC_ComponentManager<KC_ComponentsRegistry::MainComponents>;
using KC_RenderComponentManager = KC_ComponentManager<KC_ComponentsRegistry::RenderComponents>;
