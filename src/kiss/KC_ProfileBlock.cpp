#include "KC_ProfileBlock.h"

#include "KC_ProfileManager.h"

KC_ProfileBlock::KC_ProfileBlock(const char* aName)
    : myName(aName)
{
}

void KC_ProfileBlock::ResetTime()
{
    myTimes.myThreadId = std::this_thread::get_id();
    myTimes.myStartTime = KC_Time::Now();
}

void KC_ProfileBlock::RecordTime()
{
    myTimes.myEndTime = KC_Time::Now();
    KC_ProfileManager::GetManager().SetBlock(myName, myTimes);
}
