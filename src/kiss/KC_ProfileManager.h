#pragma once

#include "KC_Manager.h"
#include "KC_ProfileBlockTimes.h"

#include <mutex>
#include <unordered_map>

class KC_ProfileManager final : public KC_Manager<KC_ProfileManager>
{
public:
    void SetBlock(const char* aBlockName, const KC_ProfileBlockTimes& aBlockTimes);
    void SwapBlocks(std::unordered_map<const char*, KC_ProfileBlockTimes>& outBlocksTimes);

private:
    std::mutex myBlocksTimesMutex;
    std::unordered_map<const char*, KC_ProfileBlockTimes> myBlocksTimes;
};
