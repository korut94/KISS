#include "KC_ScopedProfileTimer.h"

KC_ScopedProfileTimer::KC_ScopedProfileTimer(const char* aTimerName)
    : myProfileTimer(aTimerName)
{
    myProfileTimer.ResetTime();
}

KC_ScopedProfileTimer::~KC_ScopedProfileTimer()
{
    myProfileTimer.RecordTime();
}
