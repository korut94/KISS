#include "KC_CircleRenderSystem.h"

#include "KC_CircleRenderer.h"
#include "KC_Transform.h"

#if IS_DEBUG_BUILD
#include "KC_Profiling.h"
#endif // IS_DEBUG_BUILD

#include <SFML/Graphics.hpp>

namespace KC_CircleRenderSystem_Private
{
    // Unique instance of circle shape to be able to draw different circles by
    // just changing its internal properties.
    static sf::CircleShape locCircleShape;
}

void KC_CircleRenderSystem::Run(sf::RenderWindow& aRenderWindow) const
{
    namespace Private = KC_CircleRenderSystem_Private;

    for (KC_Entity entity : myEntitySet)
    {
#if IS_DEBUG_BUILD
        KC_ProfileTimer getComponentTimer{ KC_ProfileTimerType::RenderSystemGetComponent };
#endif // IS_DEBUG_BUILD
        
        const KC_Transform& transform = GetComponent<KC_Transform>(entity);
        const KC_CircleRenderer& renderer = GetComponent<KC_CircleRenderer>(entity);

#if IS_DEBUG_BUILD
        getComponentTimer.RecordTime();

        KC_ProfileTimer createCircleTimer{ KC_ProfileTimerType::RenderSystemCreateDrawnable };
#endif // IS_DEBUG_BUILD

        Private::locCircleShape.setPosition(transform.myPosition);
        Private::locCircleShape.setFillColor(renderer.myFillColor);
        Private::locCircleShape.setOutlineColor(renderer.myOutlineColor);

        // It's pretty expensive changing the circle shape radius. Do it only when necessary.
        if (abs(Private::locCircleShape.getRadius() - renderer.myRadius) > 0.001f)
        {
            Private::locCircleShape.setRadius(renderer.myRadius);
        }
        
#if IS_DEBUG_BUILD
        createCircleTimer.RecordTime();
#endif // IS_DEBUG_BUILD

        {
            KC_PROFILE(RenderSystemDraw);
            aRenderWindow.draw(Private::locCircleShape);
        }
    }
}
