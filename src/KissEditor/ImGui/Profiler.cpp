#include "Profiler.h"

#include "KC_ProfileManager.h"
#include "KC_ThreadManager.h"

#include "imgui.h"

#include <unordered_map>
#include <vector>

namespace Profiler_Private
{
    bool IsGreaterBlockTime(
        const std::pair<const char*, KC_ProfileBlockTimes>& a,
        const std::pair<const char*, KC_ProfileBlockTimes>& b)
    {
        return a.second.myThreadId > b.second.myThreadId || a.second.myStartTime > b.second.myStartTime;
    }

    void SortProfileBlocks(
        const std::unordered_map<const char*, KC_ProfileBlockTimes>& someProfileBlocks, 
        std::vector<std::pair<const char*, KC_ProfileBlockTimes>>& outSortedBlocks)
    {
        outSortedBlocks.reserve(someProfileBlocks.size());

        for (auto pair : someProfileBlocks)
        {
            if (outSortedBlocks.size() == 0)
            {
                outSortedBlocks.push_back(pair);
            }
            else
            {
                const auto begin = outSortedBlocks.begin();
                auto itr = outSortedBlocks.end();
                while (itr != begin && IsGreaterBlockTime(*(itr - 1), pair))
                {
                    --itr;
                }

                outSortedBlocks.insert(itr, pair);
            }
        }
    }
}

namespace ImGui
{
namespace Editor
{
void Profiler()
{
    static std::unordered_map<const char*, KC_ProfileBlockTimes> profileBlocks;

    KC_ProfileManager& profiler = KC_ProfileManager::GetManager();
    profiler.SwapBlocks(profileBlocks);

    std::vector<std::pair<const char*, KC_ProfileBlockTimes>> sortedProfileBlocks;
    Profiler_Private::SortProfileBlocks(profileBlocks, sortedProfileBlocks);

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
                const std::uint64_t duration = (profileBlock.second.myEndTime - profileBlock.second.myStartTime).AsNanoseconds();

                ImGui::Text("%s: %dns", profileBlock.first, duration);
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

    profileBlocks.clear();
}
} // Editor
} // ImGui
