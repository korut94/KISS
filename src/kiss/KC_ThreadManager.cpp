#include "KC_ThreadManager.h"

#include "KC_Assert.h"

#if IS_DEBUG_BUILD
const char* KC_ThreadManager::GetThreadName(std::thread::id aThreadId) const
{
    auto itr = myThreadNames.find(aThreadId);
    KC_ASSERT(itr != myThreadNames.cend());

    return itr->second;
}

void KC_ThreadManager::SetThreadName(const char* aThreadName)
{
    myThreadNames[std::this_thread::get_id()] = aThreadName;
}
#endif // IS_DEBUG_BUILD
