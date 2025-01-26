#include "MC_BounceOnCollisionSystem.h"

#include "KC_Transform.h"
#include "KC_Velocity.h"

#include <algorithm>

void MC_BounceOnCollisionSystem::Run()
{
    const KC_CollisionEventSet& collisionEvents = GetCollisionEventSet();

    for (KC_Entity entity : myEntitySet)
    {
        if (!collisionEvents.HasCollision(entity))
            continue;

        KC_Velocity& velocity = GetComponent<KC_Velocity>(entity);
        velocity.myVector = -velocity.myVector;
    }
}
