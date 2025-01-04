#include "KC_ScopedProfileTimer.h"

KC_ScopedProfileTimer::KC_ScopedProfileTimer(KC_ProfileTimerType aProfileTimerType)
    : myProfileTimer(aProfileTimerType)
{
    myProfileTimer.ResetTime();
}

KC_ScopedProfileTimer::~KC_ScopedProfileTimer()
{
    myProfileTimer.RecordTime();
}
