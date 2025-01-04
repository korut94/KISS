#include "KC_Profiler.h"

#include "KC_Assert.h"

KC_Profiler* KC_Profiler::myInstance = nullptr;

KC_Profiler::KC_Profiler()
{
    KC_ASSERT(!myInstance, "An instance of KC_Profiler already exists");
    myInstance = this;
}

sf::Time KC_Profiler::GetTime(const char* aTimerName) const
{
    if (auto itr = myTimes.find(aTimerName); itr != myTimes.cend())
        return itr->second;
    
    return sf::Time();
}

void KC_Profiler::SetTime(const char* aTimerName, sf::Time aTime)
{
    myTimes[aTimerName] = aTime;
}

KC_Profiler& KC_Profiler::GetProfiler()
{
    KC_ASSERT(myInstance, "Did you forget to create the profiler?");
    return *myInstance;
}
