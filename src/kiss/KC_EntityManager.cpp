#include "KC_EntityManager.h"

#include "KC_Assert.h"

#include <limits>

KC_EntityManager::KC_EntityManager()
    : myEntityCount(0)
{
}

KC_Entity KC_EntityManager::CreateEntity()
{
    KC_ASSERT(myEntityCount != std::numeric_limits<KC_Entity>::max());
    return myEntityCount++;
}
