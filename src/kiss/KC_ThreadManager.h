#pragma once

#include "KC_Manager.h"

#include <thread>
#if IS_DEBUG_BUILD
#include <unordered_map>
#endif // IS_DEBUG_BUILD

class KC_ThreadManager final : public KC_Manager<KC_ThreadManager>
{
#if IS_DEBUG_BUILD
public:
    const char* GetThreadName(std::thread::id aThreadId) const;

    void SetThreadName(const char* aThreadName);

private:
    std::unordered_map<std::thread::id, const char*> myThreadNames;
#endif // IS_DEBUG_BUILD
};

#if IS_DEBUG_BUILD
#define KC_THREAD(aName) KC_ThreadManager::GetManager().SetThreadName(aName);
#else
#define KC_THREAD(aName)
#endif // IS_DEBUG_BUILD
