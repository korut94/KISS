#include "KC_Profiler.h"

#include "KC_Assert.h"

KC_Profiler* KC_Profiler::myInstance = nullptr;

KC_Profiler::KC_Profiler()
{
    KC_ASSERT(!myInstance, "An instance of KC_Profiler already exists");
    myInstance = this;
}

sf::Time KC_Profiler::GetTime(KC_ProfileTimerType aProfileTimerType) const
{
    return myTimes[aProfileTimerType];
}

void KC_Profiler::SetTime(KC_ProfileTimerType aProfileTimerType, sf::Time aTime)
{
    myTimes[aProfileTimerType] = aTime;
}

KC_Profiler& KC_Profiler::GetProfiler()
{
    KC_ASSERT(myInstance, "Did you forget to create the profiler?");
    return *myInstance;
}
