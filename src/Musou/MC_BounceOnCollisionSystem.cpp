#include "MC_BounceOnCollisionSystem.h"

#include "KC_CollisionEvent.h"
#include "KC_Transform.h"
#include "KC_Velocity.h"

#include <algorithm>

void MC_BounceOnCollisionSystem::Run(const std::vector<KC_CollisionEvent>& someCollisionEvents)
{
    for (KC_Entity entity : myEntitySet)
    {
        auto itr = std::find_if(someCollisionEvents.cbegin(), someCollisionEvents.cend(), [entity](const KC_CollisionEvent& anEvent)
        {
            return anEvent.myEntity == entity || anEvent.myOtherEntity == entity;
        });

        if (itr == someCollisionEvents.cend())
            continue;

        KC_Velocity& velocity = GetComponent<KC_Velocity>(entity);
        velocity.myVector = -velocity.myVector;
    }
}
