#include "KC_ApplyActiveCameraSystem.h"

#include "KC_Camera.h"

#include <SFML/Graphics/RenderWindow.hpp>

namespace KC_ApplyActiveCameraSystem_Private
{
    static sf::View locView;
    static constexpr float locUnitScale = 10.f; // pixel/meter
}

void KC_ApplyActiveCameraSystem::Run(sf::RenderWindow& aRenderWindow) const
{
    namespace Private = KC_ApplyActiveCameraSystem_Private;

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
    Private::locView.zoom(1.f / (Private::locUnitScale * camera.myZoom));

    aRenderWindow.setView(Private::locView);
}
