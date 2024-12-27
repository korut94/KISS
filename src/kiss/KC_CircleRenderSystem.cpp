#include "KC_CircleRenderSystem.h"

#include "KC_CircleRenderer.h"
#include "KC_Transform.h"

#include <SFML/Graphics.hpp>

void KC_CircleRenderSystem::Run(sf::RenderWindow& aRenderWindow) const
{
    for (KC_Entity entity : myEntitySet)
    {
        const KC_Transform& transform = GetComponent<KC_Transform>(entity);
        const KC_CircleRenderer& renderer = GetComponent<KC_CircleRenderer>(entity);

        sf::CircleShape circle;
        circle.setPosition(transform.myPosition);
        circle.setRadius(renderer.myRadius);
        circle.setFillColor(renderer.myFillColor);
        circle.setOutlineColor(renderer.myOutlineColor);

        aRenderWindow.draw(circle);
    }
}
