#pragma once

#include "KC_ComponentManager.h"
#include "KC_System.h"

template <typename T, typename... Args>
using KC_GameSystem = KC_System<KC_MainComponentManager, T, Args...>;
