#include "KC_ProfileTimer.h"

#include "KC_Profiler.h"

KC_ProfileTimer::KC_ProfileTimer(KC_ProfileTimerType aProfilerTimerType)
    : myProfilerTimerType(aProfilerTimerType)
{
}

void KC_ProfileTimer::ResetTime()
{
    myClock.restart();
}

void KC_ProfileTimer::RecordTime()
{
    KC_Profiler::GetProfiler().SetTime(myProfilerTimerType, myClock.getElapsedTime());
}
