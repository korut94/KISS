#pragma once

#include "KC_RenderSystem.h"

class KC_CircleRenderer;
class KC_Transform;

class KC_DrawCircleSystem final : public KC_RenderSystem<KC_Transform, KC_CircleRenderer>
{
    KC_SYSTEM(KC_DrawCircleSystem)

public:
    void Run(sf::RenderWindow& aRenderWindow) const;
};
