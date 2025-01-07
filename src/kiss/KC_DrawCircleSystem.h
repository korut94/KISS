#pragma once

#include "KC_DrawSystem.h"

class KC_CircleRenderer;
class KC_Transform;

namespace sf { class RenderWindow; }

class KC_DrawCircleSystem final : public KC_DrawSystem<KC_Transform, KC_CircleRenderer>
{
    using Super = KC_DrawSystem<KC_Transform, KC_CircleRenderer>;

public:
    using Super::KC_DrawSystem;

    void Run(sf::RenderWindow& aRenderWindow) const;
};
