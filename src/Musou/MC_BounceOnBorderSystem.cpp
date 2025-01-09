#include "MC_BounceOnBorderSystem.h"

#include "KC_Transform.h"
#include "KC_Velocity.h"

void MC_BounceOnBorderSystem::Run()
{
    for (KC_Entity entity : myEntitySet)
    {
        KC_Velocity& velocity = GetComponent<KC_Velocity>(entity);
        const KC_Transform& transform = GetComponent<KC_Transform>(entity);

        const sf::Vector2f& position = transform.myPosition;

        if (position.x < -1000.f || position.x > 1000.f || position.y < -500.f || position.y > 500.f)
        {
            velocity.myVector = -velocity.myVector;
        }
    }
}