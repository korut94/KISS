#include "KC_RenderViewSystem.h"

#include "KC_Camera.h"

#include <SFML/Graphics/RenderWindow.hpp>

namespace KC_RenderViewSystem_Private
{
    static sf::View locView;
}

void KC_RenderViewSystem::Run(sf::RenderWindow& aRenderWindow) const
{
    namespace Private = KC_RenderViewSystem_Private;

    if (myEntitySet.IsEmpty())
        return;

    KC_Entity cameraEntity = myEntitySet.At(0);

    const KC_Transform& transform = GetComponent<KC_Transform>(cameraEntity);
    const KC_Camera& camera = GetComponent<KC_Camera>(cameraEntity);
    
    const sf::Vector2u windowSize = aRenderWindow.getSize();

    Private::locView = aRenderWindow.getDefaultView();
    // Define world center coordinate
    Private::locView.setCenter(transform.myPosition);
    // Apply zoom
    Private::locView.zoom(1.f / camera.myZoom);

    aRenderWindow.setView(Private::locView);
}
