#pragma once

#include "KC_CollisionEvent.h"

#include <unordered_map>
#include <vector>

class KC_CollisionEventSet final
{
public:
    bool AreColliding(KC_Entity anEntity, KC_Entity anOtherEntity) const;
    bool HasCollision(KC_Entity anEntity) const;
    const std::vector<KC_CollisionEvent>& GetCollisionEvents(KC_Entity anEntity) const;
#if IS_DEBUG_BUILD
    int32_t CollisionsCount() const;
    float AvgCollisionsPerEntity() const;
    int32_t MaxCollisionsPerEntity() const;
    int32_t MinCollisionsPerEntity() const;
#endif // IS_DEBUG_BUILD

    void Clear();
    void InsertCollisionEvent(KC_Entity anEntity, KC_Entity aHitEntity, float aContactTime);

private:
    void InsertCollisionEventImpl(KC_Entity anEntity, KC_Entity aHitEntity, float aContactTime);

    std::unordered_map<KC_Entity, std::vector<KC_CollisionEvent>> myCollisionEventsMap;
};

#if IS_IMGUI
void KC_ImGui(const KC_CollisionEventSet& aCollisionEventSet);
#endif // IS_IMGUI
