#include "KC_CollisionEventSet.h"

#include "KC_Assert.h"

#include <algorithm>

#if IS_IMGUI
#include "imgui.h"
#endif // IS_IMGUI

namespace KC_CollisionEventSet_Private
{
    static constexpr const size_t locMinArraySize = 4;

    bool ContainsHitEntity(const std::vector<KC_CollisionEvent>& someCollisionEvents, KC_Entity anEntity)
    {
        const auto itr = std::find_if(someCollisionEvents.cbegin(), someCollisionEvents.cend(), [anEntity](const KC_CollisionEvent& aCollisionEvent)
        {
            return aCollisionEvent.myHitEntity == anEntity;
        });

        return itr != someCollisionEvents.cend();
    }
}

bool KC_CollisionEventSet::AreColliding(KC_Entity anEntity, KC_Entity anOtherEntity) const
{
    namespace Private = KC_CollisionEventSet_Private;

    const auto itr = myCollisionEventsMap.find(anEntity);
    if (itr == myCollisionEventsMap.cend())
        return false;

    return Private::ContainsHitEntity(itr->second, anOtherEntity);
}

bool KC_CollisionEventSet::HasCollision(KC_Entity anEntity) const
{
    return myCollisionEventsMap.find(anEntity) != myCollisionEventsMap.cend();
}

const std::vector<KC_CollisionEvent>& KC_CollisionEventSet::GetCollisionEvents(KC_Entity anEntity) const
{
    const auto itr = myCollisionEventsMap.find(anEntity);
    KC_ASSERT(itr != myCollisionEventsMap.cend());

    return itr->second;
}

#if IS_DEBUG_BUILD
int32_t KC_CollisionEventSet::CollisionsCount() const
{
    int32_t collisionsCount = 0;

    for (auto itr = myCollisionEventsMap.cbegin(), end = myCollisionEventsMap.cend(); itr != end; ++itr)
    {
        collisionsCount += itr->second.size();
    }

    return collisionsCount >> 1;
}

float KC_CollisionEventSet::AvgCollisionsPerEntity() const
{
    return static_cast<float>(CollisionsCount()) / static_cast<float>(myCollisionEventsMap.size());
}

int32_t KC_CollisionEventSet::MaxCollisionsPerEntity() const
{
    using Pair = std::pair<KC_Entity, std::vector<KC_CollisionEvent>>;
    const auto itr = std::max_element(myCollisionEventsMap.cbegin(), myCollisionEventsMap.cend(), [](const Pair& aMax, const Pair& aCurrent)
    {
        return aCurrent.second.size() > aMax.second.size();
    });
    
    return itr != myCollisionEventsMap.cend() ? itr->second.size() : 0;
}

int32_t KC_CollisionEventSet::MinCollisionsPerEntity() const
{
    using Pair = std::pair<KC_Entity, std::vector<KC_CollisionEvent>>;
    const auto itr = std::min_element(myCollisionEventsMap.cbegin(), myCollisionEventsMap.cend(), [](const Pair& aCurrent, const Pair& aMin)
    {
        return aCurrent.second.size() < aMin.second.size();
    });
    
    return itr != myCollisionEventsMap.cend() ? itr->second.size() : 0;
}
#endif // IS_DEBUG_BUILD

void KC_CollisionEventSet::Clear()
{
    myCollisionEventsMap.clear();
}

void KC_CollisionEventSet::InsertCollisionEvent(KC_Entity anEntity, KC_Entity aHitEntity, float aContactTime)
{
    // Register the collision on both the side to allow possible distinct logic for both the entites
    InsertCollisionEventImpl(anEntity, aHitEntity, aContactTime);
    InsertCollisionEventImpl(aHitEntity, anEntity, aContactTime);
}

void KC_CollisionEventSet::InsertCollisionEventImpl(KC_Entity anEntity, KC_Entity aHitEntity, float aContactTime)
{
    namespace Private = KC_CollisionEventSet_Private;

    std::vector<KC_CollisionEvent>& collisions = myCollisionEventsMap[anEntity];
    KC_ASSERT(!Private::ContainsHitEntity(collisions, aHitEntity));

    collisions.reserve(Private::locMinArraySize);
    collisions.emplace_back(aHitEntity, aContactTime);
}

#if IS_IMGUI
void KC_ImGui(const KC_CollisionEventSet& aCollisionEventSet)
{
#if IS_DEBUG_BUILD
    ImGui::Text("Count: %d", aCollisionEventSet.CollisionsCount());
    ImGui::Text("Avg: %.2f", aCollisionEventSet.AvgCollisionsPerEntity());
    ImGui::Text("Min-Max: [%d, %d]", aCollisionEventSet.MinCollisionsPerEntity(), aCollisionEventSet.MaxCollisionsPerEntity());
#endif // IS_DEBUG_BUILD
}
#endif // IS_IMGUI
