#include "KC_ResolveCollisionSystem.h"

#include "KC_CollisionEvent.h"
#include "KC_Intersection.h"
#include "KC_RectCollider.h"
#include "KC_SpatialGrid.h"
#include "KC_Transform.h"

#include <algorithm>

void KC_ResolveCollisionSystem::Run(const KC_SpatialGrid& aSpatialGrid, std::vector<KC_CollisionEvent>& outSomeCollisionEvents) const
{
    outSomeCollisionEvents.clear();

    for (KC_Entity entity : myEntitySet)
    {
        const KC_Transform& transform = GetComponent<KC_Transform>(entity);
        const KC_RectCollider& collider = GetComponent<KC_RectCollider>(entity);

        const KC_FloatRect bound = transform * collider.myBound;

        std::vector<KC_Entity> neighborEntities;
        aSpatialGrid.GetEntitiesInsideBound(bound, neighborEntities);

        for (KC_Entity otherEntity : neighborEntities)
        {
            if (entity == otherEntity)
                continue;

            // Avoid to register the same collision twice but with the entities swapped
            auto itr = std::find_if(outSomeCollisionEvents.cbegin(), outSomeCollisionEvents.cend(), [entity, otherEntity](const KC_CollisionEvent& anEvent)
            {
                return anEvent.myEntity == otherEntity && anEvent.myOtherEntity == entity;
            });

            if (itr != outSomeCollisionEvents.cend())
                continue;

            const KC_Transform& otherTransform = GetComponent<KC_Transform>(otherEntity);
            const KC_RectCollider& otherCollider = GetComponent<KC_RectCollider>(otherEntity);

            const KC_FloatRect otherBound = otherTransform * otherCollider.myBound;

            if (KC_Intersection::RectVsRect(bound, otherBound))
            {
                outSomeCollisionEvents.push_back({ entity, otherEntity });
            }
        }
    }
}
