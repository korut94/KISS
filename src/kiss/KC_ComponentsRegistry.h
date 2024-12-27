#pragma once

#include "KC_CircleRenderer.h"
#include "KC_RectangleRenderer.h"
#include "KC_Transform.h"

#include <tuple>

namespace KC_ComponentsRegistry
{
     // Add new components here
    using Components = std::tuple<
        KC_CircleRenderer,
        KC_RectangleRenderer,
        KC_Transform
    >;
    
    using MainComponents = Components;
    // Add components to use in the render thread here
    using RenderComponents = std::tuple<
        KC_CircleRenderer,
        KC_Transform
    >;
}
