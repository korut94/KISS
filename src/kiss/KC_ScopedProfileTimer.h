#pragma once

#include "KC_ProfileTimer.h"

class KC_ScopedProfileTimer final
{
public:
    KC_ScopedProfileTimer(KC_ProfileTimerType aProfileTimerType);
    ~KC_ScopedProfileTimer();

private:
    KC_ProfileTimer myProfileTimer;
};
