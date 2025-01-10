#pragma once

#include "KC_RenderSystem.h"

class KC_DrawCanvasSystem final : public KC_RenderSystem<KC_Canvas>
{
    KC_SYSTEM(KC_DrawCanvasSystem)

public:
    void Run(sf::RenderWindow& aRenderWindow) const;
};
