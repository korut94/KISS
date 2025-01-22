#include "MC_BounceOnCollisionSystem.h"

#include "KC_CollisionEvent.h"
#include "KC_Transform.h"
#include "KC_Velocity.h"

#include <algorithm>

void MC_BounceOnCollisionSystem::Run()
{
    const std::vector<KC_CollisionEvent>& collisionEvents = GetCollisionEvents();

    for (KC_Entity entity : myEntitySet)
    {
        auto itr = std::find_if(collisionEvents.cbegin(), collisionEvents.cend(), [entity](const KC_CollisionEvent& anEvent)
        {
            return anEvent.myEntity == entity || anEvent.myOtherEntity == entity;
        });

        if (itr == collisionEvents.cend())
            continue;

        KC_Velocity& velocity = GetComponent<KC_Velocity>(entity);
        velocity.myVector = -velocity.myVector;
    }
}
