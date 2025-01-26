#include "KC_DetectCollisionSystem.h"

#include "KC_Defines.h"
#include "KC_Intersection.h"
#include "KC_Math.h"
#include "KC_Profiling.h"
#include "KC_RectCollider.h"
#include "KC_SpatialGrid.h"
#include "KC_Transform.h"

void KC_DetectCollisionSystem::Run()
{
    const std::vector<KC_SpatialGrid>& spatialGrids = GetSpatialGrids();
    KC_ASSERT(spatialGrids.size() > 0);

    const KC_SpatialGrid& spatialGrid = spatialGrids[0];
    KC_CollisionEventSet& collisionEventSet = GetCollisionEventSet();

    collisionEventSet.Clear();

    for (KC_Entity entity : myEntitySet)
    {
        const KC_Transform& transform = GetComponent<KC_Transform>(entity);
        const KC_RectCollider& collider = GetComponent<KC_RectCollider>(entity);
        const KC_Velocity& velocity = GetComponent<KC_Velocity>(entity);

        std::vector<KC_Entity> nearbyEntities;
        {
            KC_SYSTEM_PROFILE(KC_DetectCollisionSystem, "GetNearby");
            spatialGrid.GetNearbyEntities(entity, nearbyEntities);
        }

        const KC_FloatRect bound = transform * collider.myBound;
        
        KC_SYSTEM_PROFILE(KC_DetectCollisionSystem, "CheckNearby");
        for (KC_Entity nearbyEntity : nearbyEntities)
        {
            {
                KC_SYSTEM_PROFILE(KC_DetectCollisionSystem, "AvoidDoubleInsertion");
                // Avoid to register the same collision twice but with just the entities swapped
                if (collisionEventSet.AreColliding(entity, nearbyEntity))
                    continue;
            }

            const KC_Transform& otherTransform = GetComponent<KC_Transform>(nearbyEntity);
            const KC_RectCollider& otherCollider = GetComponent<KC_RectCollider>(nearbyEntity);
            const KC_Velocity& otherVelocity = GetComponent<KC_Velocity>(nearbyEntity);

            const KC_FloatRect otherBound = otherTransform * otherCollider.myBound;

            KC_SYSTEM_PROFILE(KC_DetectCollisionSystem, "CollisionCheck");
            float contactTime = std::numeric_limits<float>::max();
            const bool isColliding = KC_Intersection::MovingRectVsMovingRect
            (
                bound, otherBound,
                velocity.myVector, otherVelocity.myVector,
                contactTime
            );
            const bool areCollidingInOneFrame = contactTime < GetElapsedTime() || KC_Math::AreValuesClose(contactTime, GetElapsedTime());

            if (areCollidingInOneFrame)
            {
                collisionEventSet.InsertCollisionEvent(entity, nearbyEntity, contactTime);
            }
        }
    }
}
