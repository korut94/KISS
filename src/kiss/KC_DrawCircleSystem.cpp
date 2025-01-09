#include "KC_DrawCircleSystem.h"

#include "KC_CircleRenderer.h"
#include "KC_Profiling.h"
#include "KC_Transform.h"

#include <SFML/Graphics.hpp>

namespace KC_CircleRenderSystem_Private
{
    // Unique instance of circle shape to be able to draw different circles by
    // just changing its internal properties.
    static sf::CircleShape locCircleShape;
}

void KC_DrawCircleSystem::Run(sf::RenderWindow& aRenderWindow) const
{
    namespace Private = KC_CircleRenderSystem_Private;

    for (KC_Entity entity : myEntitySet)
    {
        const KC_Transform& transform = GetComponent<KC_Transform>(entity);
        const KC_CircleRenderer& renderer = GetComponent<KC_CircleRenderer>(entity);

        KC_PROFILE_STEP("KC_DrawCircleSystem", "Draw")
        Private::locCircleShape.setPosition(transform.myPosition);
        // It's pretty expensive changing the circle shape radius. Do it only when necessary.
        if (abs(Private::locCircleShape.getRadius() - renderer.myRadius) > 0.001f)
        {
            Private::locCircleShape.setRadius(renderer.myRadius);
        }

        // TODO: Implement a view culling

        Private::locCircleShape.setFillColor(renderer.myFillColor);
        Private::locCircleShape.setOutlineColor(renderer.myOutlineColor);

        aRenderWindow.draw(Private::locCircleShape);
    }
}
