#pragma once

#include "KC_RenderSystem.h"

class KC_Camera;
class KC_Transform;

class KC_RenderViewSystem final : public KC_RenderSystem<KC_Transform, KC_Camera>
{
    KC_SYSTEM(KC_RenderViewSystem)

public:
    void Run(sf::RenderWindow& aRenderWindow) const;
};
