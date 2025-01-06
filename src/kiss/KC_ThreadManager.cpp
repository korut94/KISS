#include "KC_ThreadManager.h"

#if IS_DEBUG_BUILD
#include <algorithm>

KC_ThreadManager::KC_ThreadManager()
{
    myThreadMetadata.reserve(4);
}

const char *KC_ThreadManager::GetThreadName(std::thread::id aThreadId) const
{
    auto itr = FindThreadMetadata(aThreadId);
    KC_ASSERT(itr != myThreadMetadata.cend());

    return itr->myThreadName;
}

std::vector<KC_ThreadManager::ThreadMetadata>::const_iterator KC_ThreadManager::FindThreadByName(const char* aThreadName) const
{
    return std::find_if(myThreadMetadata.cbegin(), myThreadMetadata.cend(), [aThreadName](const ThreadMetadata& aThreadMetadata)
    {
        return aThreadMetadata.myThreadName == aThreadName;
    });
}

std::vector<KC_ThreadManager::ThreadMetadata>::const_iterator KC_ThreadManager::FindThreadMetadata(std::thread::id aThreadId) const
{
    return std::find_if(myThreadMetadata.cbegin(), myThreadMetadata.cend(), [aThreadId](const ThreadMetadata& aThreadMetadata)
    {
        return aThreadMetadata.myThreadId == aThreadId;
    });
}
#endif // IS_DEBUG_BUILD
