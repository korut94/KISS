#include "KC_SpatialGridSystem.h"

#include "KC_RectCollider.h"
#include "KC_SpatialGrid.h"
#include "KC_Transform.h"

#include <SFML/Graphics/Transform.hpp>

void KC_SpatialGridSystem::Run(KC_SpatialGrid& aSpatialGrid)
{
    aSpatialGrid.Clear();

    for (KC_Entity entity : myEntitySet)
    {
        const KC_Transform& transform = GetComponent<KC_Transform>(entity);
        const KC_RectCollider& collider = GetComponent<KC_RectCollider>(entity);

        const sf::Transform transformationMatrix
        {
            transform.myScale.x, 0, transform.myPosition.x,
            0, transform.myScale.y, transform.myPosition.y,
            0, 0, 1
        };

        aSpatialGrid.InsertEntity(entity, transformationMatrix * collider.myBound);
    }
}
