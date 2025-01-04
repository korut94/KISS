#pragma once

#if IS_DEBUG_BUILD
#include "KC_ScopedProfileTimer.h"
#define KC_PROFILE(aProfileTimerType) KC_ScopedProfileTimer scopedProfileTimer{ KC_ProfileTimerType::aProfileTimerType };
#else
#define KC_PROFILE(aProfileTimerType)
#endif // IS_DEBUG_BUILD
