#pragma once

#include "KC_RenderSystem.h"

class KC_CircleRenderer;
class KC_Transform;

namespace sf { class RenderWindow; }

class KC_CircleRenderSystem final : public KC_RenderSystem<KC_Transform, KC_CircleRenderer>
{
    using Super = KC_RenderSystem<KC_Transform, KC_CircleRenderer>;

public:
    using Super::KC_RenderSystem;

    void Run(sf::RenderWindow& aRenderWindow) const;
};
