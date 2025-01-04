#include "KC_ScopedProfileTimer.h"

#include "KC_Profiler.h"

KC_ScopedProfileTimer::KC_ScopedProfileTimer(KC_ProfileTimerType profileTimerType)
    : myProfilerTimerType(profileTimerType)
{
}

KC_ScopedProfileTimer::~KC_ScopedProfileTimer()
{
    KC_Profiler::GetProfiler().SetTime(myProfilerTimerType, myClock.getElapsedTime());
}
