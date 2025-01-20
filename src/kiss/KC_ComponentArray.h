#pragma once

#include "KC_Assert.h"
#include "KC_EntitySet.h"

#include <vector>

class KC_ComponentArray {};

template <typename T>
class KC_ComponentArrayImpl final : public KC_ComponentArray
{
public:
    KC_ComponentArrayImpl();

    const KC_EntitySet& GetEntitySet() const { return myEntitySet; }

    const T& GetComponent(std::size_t anIndex) const;
    T& GetComponent(std::size_t anIndex);
    const T& GetComponent(KC_Entity anEntity) const;
    T& GetComponent(KC_Entity anEntity);
    
    template <typename... Args>
    T& InsertComponent(KC_Entity anEntity, Args&&... args);

    KC_ComponentArrayImpl<T>& operator=(const KC_ComponentArrayImpl<T>& anOther);

private:
    KC_EntitySet myEntitySet;
    std::vector<T> myComponentInstances;
};

template <typename T>
KC_ComponentArrayImpl<T>::KC_ComponentArrayImpl()
{
    // TODO: A lot of memory crashes due to an empty array. May we should allocate from the beginning
    // a dedicated room for the components.
    myComponentInstances.reserve(10);
}

template <typename T>
const T& KC_ComponentArrayImpl<T>::GetComponent(std::size_t anIndex) const
{
    KC_ASSERT(anIndex < myComponentInstances.size(), "Component index out of bound");
    return myComponentInstances[anIndex];
}

template <typename T>
T& KC_ComponentArrayImpl<T>::GetComponent(std::size_t anIndex)
{
    KC_ASSERT(anIndex < myComponentInstances.size(), "Component index out of bound");
    return myComponentInstances[anIndex];
}

template <typename T>
const T& KC_ComponentArrayImpl<T>::GetComponent(KC_Entity anEntity) const
{
    KC_ASSERT(myEntitySet.Contains(anEntity), "No component for provided entity");
    return myComponentInstances[myEntitySet.GetIndex(anEntity)];
}

template <typename T>
T& KC_ComponentArrayImpl<T>::GetComponent(KC_Entity anEntity)
{
    KC_ASSERT(myEntitySet.Contains(anEntity), "No component for provided entity");
    return myComponentInstances[myEntitySet.GetIndex(anEntity)];
}

template <typename T>
template <typename... Args>
T& KC_ComponentArrayImpl<T>::InsertComponent(KC_Entity anEntity, Args&&... args)
{
    KC_ASSERT(!myEntitySet.Contains(anEntity), "Provided entity has already an instance of the specified component");

    myEntitySet.Insert(anEntity);
    return myComponentInstances.emplace_back(std::forward<Args>(args)...);
}

template <typename T>
KC_ComponentArrayImpl<T>& KC_ComponentArrayImpl<T>::operator=(const KC_ComponentArrayImpl<T>& anOther)
{
    myEntitySet = anOther.myEntitySet;
    myComponentInstances = anOther.myComponentInstances;

    return *this;
}
