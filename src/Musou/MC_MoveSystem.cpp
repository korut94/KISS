#include "MC_MoveSystem.h"

#include "KC_Profiling.h"

#include <algorithm>

namespace KC_MoveSystem_Private
{
    float SafeCollisionTimeProgress(float aContactTime, float anElapsedTime)
    {
        return std::min(aContactTime - anElapsedTime * 0.01f, 0.f);
    }
}

void MC_MoveSystem::Run()
{
    namespace Private = KC_MoveSystem_Private;

    KC_SpatialGrid& spatialGrid = GetSpatialGrid(0);

    for (KC_Entity entity : myEntitySet)
    {
        KC_Transform& transform = GetComponent<KC_Transform>(entity);
        const KC_Velocity& velocity = GetComponent<KC_Velocity>(entity);
        const KC_RectCollider& collider = GetComponent<KC_RectCollider>(entity);
        const KC_FloatRect bound = transform * collider.myBound;

        float time = GetElapsedTime();

        if (HasCollision(entity))
        {
            const std::vector<KC_CollisionEvent>& events = GetCollisionEvents(entity);
            const auto itr = std::min_element(events.cbegin(), events.cend(), [](const KC_CollisionEvent& aCurrent, const KC_CollisionEvent& aMin)
            {
                return aCurrent.myContactTime < aMin.myContactTime;
            });

            time = Private::SafeCollisionTimeProgress(itr->myContactTime, time);
        }

        transform.myPosition += velocity.myVector * time;

        KC_SYSTEM_PROFILE(MC_MoveSystem, "SpatialGridUpdate");
        spatialGrid.UpdateEntity(entity, bound);
    }
}
