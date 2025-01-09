#pragma once

#include "KC_Camera.h"
#include "KC_CircleRenderer.h"
#include "KC_RectCollider.h"
#include "KC_Transform.h"
#include "KC_Velocity.h"

#include <tuple>

namespace KC_ComponentsRegistry
{
     // Add new components here
    using Components = std::tuple<
        KC_Camera,
        KC_CircleRenderer,
        KC_RectCollider,
        KC_Transform,
        KC_Velocity
    >;
    
    using MainComponents = Components;
    // Add components to use in the render thread here
    using RenderComponents = std::tuple<
        KC_Camera,
        KC_CircleRenderer,
        KC_Transform
    >;
}
