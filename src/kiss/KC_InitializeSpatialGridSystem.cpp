#include "KC_InitializeSpatialGridSystem.h"

#include "KC_RectCollider.h"
#include "KC_SpatialGrid.h"
#include "KC_Transform.h"

#include <SFML/Graphics/Transform.hpp>

void KC_InitializeSpatialGridSystem::Run(KC_SpatialGrid& aSpatialGrid)
{
    aSpatialGrid.Clear();

    for (KC_Entity entity : myEntitySet)
    {
        const KC_Transform& transform = GetComponent<KC_Transform>(entity);
        const KC_RectCollider& collider = GetComponent<KC_RectCollider>(entity);

        aSpatialGrid.InsertEntity(entity, transform * collider.myBound);
    }
}
