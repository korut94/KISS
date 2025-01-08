#pragma once

#include "KC_DrawSystem.h"

class KC_CircleRenderer;
class KC_Transform;

class KC_DrawCircleSystem final : public KC_DrawSystem<KC_Transform, KC_CircleRenderer>
{
    KC_SYSTEM(KC_DrawCircleSystem)

public:
    void Run(sf::RenderWindow& aRenderWindow) const;
};
