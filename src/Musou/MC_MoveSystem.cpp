#include "MC_MoveSystem.h"

void MC_MoveSystem::Run()
{
    for (KC_Entity entity : myEntitySet)
    {
        KC_Transform& transform = GetComponent<KC_Transform>(entity);
        const KC_Velocity& velocity = GetComponent<KC_Velocity>(entity);

        transform.myPosition = transform.myPosition + velocity.myVector * GetElapsedTime();
    }
}
