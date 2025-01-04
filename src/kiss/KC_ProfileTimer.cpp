#include "KC_ProfileTimer.h"

#include "KC_Profiler.h"

KC_ProfileTimer::KC_ProfileTimer(const char* aTimerName)
    : myTimerName(aTimerName)
{
}

void KC_ProfileTimer::ResetTime()
{
    myClock.restart();
}

void KC_ProfileTimer::RecordTime()
{
    KC_Profiler::GetProfiler().SetTime(myTimerName, myClock.getElapsedTime());
}
