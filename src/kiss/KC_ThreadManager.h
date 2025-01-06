#pragma once

#include "KC_Assert.h"
#include "KC_Defines.h"
#include "KC_Manager.h"

#include <thread>
#if IS_DEBUG_BUILD
#include <vector>
#endif // IS_DEBUG_BUILD

// TODO: No thread-safe 
class KC_ThreadManager final : public KC_Manager<KC_ThreadManager>
{
#if IS_DEBUG_BUILD
    struct ThreadMetadata
    {
        std::thread::id myThreadId;
        const char* myThreadName;
    };
#endif // IS_DEBUG_BUILD

public:
#if IS_DEBUG_BUILD
    KC_ThreadManager();
#endif // IS_DEBUG_BUILD

    // TODO: We assumme the thread cannot terminate before the end of the program. Thinking if it would be always true
    // and, in case it's not, plan to wrap aFunction inside an other lamda to perform some clean-up steps after it terminates.
    template <typename TFunc>
    std::thread CreateThread(TFunc&& aFunction, const char* aThreadName);

#if IS_DEBUG_BUILD
    const char* GetThreadName(std::thread::id aThreadId) const;

private:
    std::vector<ThreadMetadata>::const_iterator FindThreadByName(const char* aThreadName) const;
    std::vector<ThreadMetadata>::const_iterator FindThreadMetadata(std::thread::id aThreadId) const;

    std::vector<ThreadMetadata> myThreadMetadata;
#endif // IS_DEBUG_BUILD
};

template <typename TFunc>
std::thread KC_ThreadManager::CreateThread(TFunc&& aFunction, const char* aThreadName)
{
#if IS_DEBUG_BUILD
    KC_ASSERT(FindThreadByName(aThreadName) == myThreadMetadata.cend(), "Thread name already assigned");

    std::thread thread{ aFunction };
    myThreadMetadata.push_back({ thread.get_id(), aThreadName });
    
    return thread;
#else
    KC_UNUSED(aThreadName);
    return std::thread(aFunction);
#endif // IS_DEBUG_BUILD
}
