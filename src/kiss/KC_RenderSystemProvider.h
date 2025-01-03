#pragma once

#include "KC_ComponentManager.h"
#include "KC_TemplateHelper.h"

#include <SFML/System/Time.hpp>

#include <condition_variable>
#include <mutex>
#include <thread>

using namespace KC_TemplateHelper;

namespace sf { class RenderWindow; }

class KC_RenderSystemProvider final
{
    enum class RenderThreadState : std::uint8_t { Ready, Run, Stop, UpdateFrame, Wait };

public:
    KC_RenderSystemProvider(sf::RenderWindow& aRenderWindow);
    ~KC_RenderSystemProvider();

    std::unique_lock<std::mutex> UpdateFrame();
    void SetComponents(const KC_MainComponentManager& aMainComponentManager);
#if IS_IMGUI
    void ImGuiUpdate(sf::Time elapsedTime);
#endif // IS_IMGUI
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
    template <typename Tuple>
    void GetEntitySet(KC_EntitySet& outEntitySet) const;
    template <typename... Args>
    void GetEntitySetImpl(KC_EntitySet& outEntitySet, UnpackedTuple<Args...>) const;

    std::thread myRenderThread;
    sf::RenderWindow& myRenderWindow;

    RenderThreadState myRenderThreadState;
    std::mutex myStateMutex;
    std::condition_variable myStateConditionVariable;

    KC_RenderComponentManager myComponentManager;
};

template <typename TSystem>
void KC_RenderSystemProvider::RunSystem() const
{
    KC_EntitySet entitySet;
    GetEntitySet<typename TSystem::Components>(entitySet);

    TSystem system { entitySet, myComponentManager };
    system.Run(myRenderWindow);
}

template <typename Tuple>
void KC_RenderSystemProvider::GetEntitySet(KC_EntitySet& outEntitySet) const
{
    GetEntitySetImpl(outEntitySet, Unpack<Tuple>{});
}

template <typename... Args>
void KC_RenderSystemProvider::GetEntitySetImpl(KC_EntitySet& outEntitySet, UnpackedTuple<Args...>) const
{
    myComponentManager.GetEntitySet<Args...>(outEntitySet);
}
