#include "MC_MoveSystem.h"

#include "KC_Profiling.h"

void MC_MoveSystem::Run()
{
    KC_SpatialGrid& spatialGrid = GetSpatialGrid(0);

    for (KC_Entity entity : myEntitySet)
    {
        KC_Transform& transform = GetComponent<KC_Transform>(entity);
        const KC_Velocity& velocity = GetComponent<KC_Velocity>(entity);
        const KC_RectCollider& collider = GetComponent<KC_RectCollider>(entity);

        transform.myPosition = transform.myPosition + velocity.myVector * GetElapsedTime();

        KC_PROFILE_STEP("MC_MoveSystem", "SpatialGridUpdate");
        spatialGrid.UpdateEntity(entity, transform * collider.myBound);
    }
}
