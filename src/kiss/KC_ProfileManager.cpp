#include "KC_ProfileManager.h"

#include "KC_Assert.h"

void KC_ProfileManager::SetBlock(const char* aBlockName, const KC_ProfileBlockTimes& aBlockTimes)
{
    std::lock_guard lock{ myBlocksTimesMutex };
    myBlocksTimes[aBlockName] = aBlockTimes;
}

void KC_ProfileManager::SwapBlocks(std::unordered_map<const char*, KC_ProfileBlockTimes>& outBlocksTimes)
{
    std::lock_guard lock{ myBlocksTimesMutex };
    myBlocksTimes.swap(outBlocksTimes);
}
