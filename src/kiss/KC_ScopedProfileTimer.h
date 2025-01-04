#pragma once

#include "KC_ProfileTimer.h"

class KC_ScopedProfileTimer final
{
public:
    KC_ScopedProfileTimer(const char* aTimerName);
    ~KC_ScopedProfileTimer();

private:
    KC_ProfileTimer myProfileTimer;
};
