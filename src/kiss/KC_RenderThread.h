#pragma once

#include "KC_RenderSystemProvider.h"

#include <SFML/System/Time.hpp>

#include <condition_variable>
#include <mutex>
#include <thread>

namespace sf { class RenderWindow; }

class KC_RenderThread final
{
    enum class State : std::uint8_t { Ready, Run, Stop, UpdateFrame, Wait };

public:
    KC_RenderThread(sf::RenderWindow& aRenderWindow);
    ~KC_RenderThread();

    std::unique_lock<std::mutex> UpdateFrame();
    void SetComponents(const KC_MainComponentManager& aMainComponentManager);
    void ImGuiUpdate(sf::Time elapsedTime);
    void Ready(std::unique_lock<std::mutex>& aLock);

private:
    void ImGuiInit();

    void Run();
    State GetState();
    void Wait();
    void Render() const;

    void StopAndWait();

    std::thread myThread;
    sf::RenderWindow& myRenderWindow;
    KC_RenderSystemProvider myRenderSystemProvider;

    State myState;
    std::mutex myStateMutex;
    std::condition_variable myStateConditionVariable;
};
