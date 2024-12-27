#pragma once

#include "KC_RenderSystemProvider.h"

#include <SFML/System/Time.hpp>

#include <condition_variable>
#include <mutex>
#include <thread>

namespace sf { class RenderWindow; }

class KC_RenderThread final
{
    enum class State : std::uint8_t { ReadyToStart, Run, Stop, UpdateFrame, Wait };

public:
    KC_RenderThread(sf::RenderWindow& aRenderWindow);

    void UpdateFrame(const KC_MainComponentManager& aMainComponentManager, sf::Time elapsedTime);
    void StopAndWait();

private:
    void Run();
    State GetState();
    void Wait();
    void Render() const;

    std::thread myThread;
    sf::RenderWindow& myRenderWindow;
    KC_RenderSystemProvider myRenderSystemProvider;

    State myState;
    std::mutex myStateMutex;
    std::condition_variable myStateConditionVariable;
};
