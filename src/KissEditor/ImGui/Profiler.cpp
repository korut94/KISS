#include "Profiler.h"

#include "KC_ProfileManager.h"
#include "KC_ThreadManager.h"

#include "imgui.h"

#include <string>
#include <unordered_map>
#include <vector>

namespace Profiler_Private
{
    static constexpr const std::uint64_t locMaxNanoseconds = 10000;
    static constexpr const std::uint64_t locMaxMicroseconds = 1000;
    static constexpr const char* locNanosecondsUnit = "ns";
    static constexpr const char* locMicrosecondsUnit = "us";
    static constexpr const char* locMillisecondsUnit = "ms";

    void SortProfileBlocks(
        const std::unordered_map<const char*, KC_ProfileBlockTimes>& someProfileBlocks, 
        std::vector<std::pair<const char*, KC_ProfileBlockTimes>>& outSortedBlocks)
    {
        outSortedBlocks.reserve(someProfileBlocks.size());

        for (auto pair : someProfileBlocks)
        {
            const auto end = outSortedBlocks.end();

            auto itr = outSortedBlocks.begin();
            while (itr != end && pair.first != itr->first) { ++itr; }
            
            if (itr != end)
            {
                itr->second = pair.second;
                continue;
            }
            
            itr = outSortedBlocks.begin();
            while (itr != end && pair.second > itr->second) { ++itr; }

            outSortedBlocks.insert(itr, pair);
        }
    }
}

namespace ImGui
{
namespace Editor
{
void Profiler()
{
    namespace Private = Profiler_Private;

    static std::vector<std::pair<const char*, KC_ProfileBlockTimes>> sortedProfileBlocks;

    KC_ProfileManager& profiler = KC_ProfileManager::GetManager();
    
    std::unordered_map<const char*, KC_ProfileBlockTimes> profileBlocks;
    profiler.SwapBlocks(profileBlocks);

    Private::SortProfileBlocks(profileBlocks, sortedProfileBlocks);

    std::size_t index = 0;
    const std::size_t sortedProfileBlocksCount = sortedProfileBlocks.size();

    ImGui::Begin("Profiler");
    while (index < sortedProfileBlocksCount)
    {
        const std::thread::id threadId = sortedProfileBlocks[index].second.myThreadId;

        const char* threadName = KC_ThreadManager::GetManager().GetThreadName(threadId);
        if (ImGui::CollapsingHeader(threadName))
        {
            while (index < sortedProfileBlocksCount && sortedProfileBlocks[index].second.myThreadId == threadId)
            {
                const std::pair<const char*, KC_ProfileBlockTimes>& profileBlock = sortedProfileBlocks[index++];
                const KC_Time duration = profileBlock.second.myEndTime - profileBlock.second.myStartTime;

                float value = 0.f;
                std::string unit;

                if (duration.AsNanoseconds() < Private::locMaxNanoseconds)
                {
                    value = static_cast<float>(duration.AsNanoseconds());
                    unit = Private::locNanosecondsUnit;
                }
                else if (duration.AsMicroseconds() < Private::locMaxMicroseconds)
                {
                    value = static_cast<float>(duration.AsNanoseconds()) / 1000.f;
                    unit = Private::locMicrosecondsUnit;
                }
                else
                {
                    value = static_cast<float>(duration.AsMicroseconds()) / 1000.f;
                    unit = Private::locMillisecondsUnit;
                }

                ImGui::Text("%s: %.2f%s", profileBlock.first, value, unit.c_str());
            }
        }
        else
        {
            // Move to next thread profile blocks
            while (index < sortedProfileBlocksCount && sortedProfileBlocks[index].second.myThreadId == threadId)
            {
                ++index;
            }
        }
    }
    ImGui::End();
}
} // Editor
} // ImGui
