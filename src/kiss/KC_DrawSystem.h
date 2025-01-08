#pragma once

#include "KC_ComponentManager.h"
#include "KC_System.h"

template <typename T, typename... Args>
using KC_DrawSystem = KC_System<const KC_RenderComponentManager, T, Args...>;

namespace sf { class RenderWindow; }
