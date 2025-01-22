#pragma once

#include "KC_ComponentManager.h"
#include "KC_Profiling.h"
#include "KC_SystemProvider.h"

#include <SFML/System/Time.hpp>

#include <condition_variable>
#include <mutex>
#include <thread>

namespace sf { class RenderWindow; }

class KC_RenderSystemProvider final : public KC_SystemProvider<KC_RenderComponentManager>
{
    enum class RenderThreadState : uint8_t { Ready, Run, Stop, UpdateFrame, Wait };

public:
    KC_RenderSystemProvider(sf::RenderWindow& aRenderWindow);
    ~KC_RenderSystemProvider();

    std::unique_lock<std::mutex> UpdateFrame();
    void SetComponents(const KC_MainComponentManager& aMainComponentManager);
    void Ready(std::unique_lock<std::mutex>& aLock);

private:
#if IS_IMGUI
    void ImGuiInit();
#endif // IS_IMGUI

    void RenderThread();
    RenderThreadState GetState();
    void Wait();
    void Render() const;
    void StopAndWait();

    template <typename TSystem>
    void RunSystem() const;

    std::thread myRenderThread;
    sf::RenderWindow& myRenderWindow;

    RenderThreadState myRenderThreadState;
    std::mutex myStateMutex;
    std::condition_variable myStateConditionVariable;
};

template <typename TSystem>
void KC_RenderSystemProvider::RunSystem() const
{
    KC_EntitySet entitySet;
    {
        KC_PROFILE(TSystem::GetEntitiesFetchTag())
        GetEntitySet<typename TSystem::Components>(entitySet);
    }
    
    TSystem system{ entitySet, myComponentManager };
    {
        KC_PROFILE(TSystem::GetRunTag())
        system.Run(myRenderWindow);
    }
}
