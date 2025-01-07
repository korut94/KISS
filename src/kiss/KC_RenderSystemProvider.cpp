#include "KC_RenderSystemProvider.h"

#include "KC_Assert.h"
#include "KC_Defines.h"
#include "KC_DrawCircleSystem.h"
#include "KC_ThreadManager.h"

#if IS_IMGUI
#include "imgui.h"
#include "imgui-SFML.h"
#endif // IS_IMGUI

#include <SFML/Graphics.hpp>

KC_RenderSystemProvider::KC_RenderSystemProvider(sf::RenderWindow& aRenderWindow)
    : myRenderWindow(aRenderWindow)
    , myRenderThreadState(RenderThreadState::Run)
{
#if IS_IMGUI
    ImGuiInit();
#endif // IS_IMGUI
    KC_UNUSED(myRenderWindow.setActive(false));
    myRenderThread = std::move(std::thread(&KC_RenderSystemProvider::RenderThread, this));
}

KC_RenderSystemProvider::~KC_RenderSystemProvider()
{
    StopAndWait();
#if IS_IMGUI
    ImGui::SFML::Shutdown();
#endif // IS_IMGUI
}

std::unique_lock<std::mutex> KC_RenderSystemProvider::UpdateFrame()
{
    std::unique_lock lock { myStateMutex };
    
    myRenderThreadState = RenderThreadState::UpdateFrame;
    myStateConditionVariable.wait(lock, [this]{ return myRenderThreadState == RenderThreadState::Wait; });

    return lock;
}

void KC_RenderSystemProvider::SetComponents(const KC_MainComponentManager& aMainComponentManager)
{
    aMainComponentManager.AssignComponents(myComponentManager);
}

#if IS_IMGUI
void KC_RenderSystemProvider::ImGuiUpdate(KC_Time anElapsedTime)
{
    ImGui::SFML::Update(myRenderWindow, sf::microseconds(anElapsedTime.AsMicroseconds()));
}
#endif // IS_IMGUI

void KC_RenderSystemProvider::Ready(std::unique_lock<std::mutex>& aLock)
{
    KC_ASSERT(aLock.owns_lock());
    myRenderThreadState = RenderThreadState::Ready;

    aLock.unlock();
    myStateConditionVariable.notify_one();

    aLock.lock();
    myStateConditionVariable.wait(aLock, [this]{ return myRenderThreadState == RenderThreadState::Run; });
}

#if IS_IMGUI
void KC_RenderSystemProvider::ImGuiInit()
{
    KC_ASSERT(ImGui::SFML::Init(myRenderWindow));
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}
#endif // IS_IMGUI

void KC_RenderSystemProvider::RenderThread()
{
    KC_THREAD("Render Thread")

    myRenderWindow.setActive(true);

    while (myRenderWindow.isOpen())
    {
        switch (GetState())
        {
        case RenderThreadState::UpdateFrame: Wait();
        case RenderThreadState::Run: Render(); break;
        case RenderThreadState::Stop: myRenderWindow.close(); break;
        }
    }
}

KC_RenderSystemProvider::RenderThreadState KC_RenderSystemProvider::GetState()
{
    std::lock_guard lock { myStateMutex };
    return myRenderThreadState;
}

void KC_RenderSystemProvider::Wait()
{
    {
        std::lock_guard lock { myStateMutex };
        myRenderThreadState = RenderThreadState::Wait;
    }
    myStateConditionVariable.notify_one();

    {
        std::unique_lock lock { myStateMutex };
        myStateConditionVariable.wait(lock, [this]{ return myRenderThreadState == RenderThreadState::Ready; });

        myRenderThreadState = RenderThreadState::Run;
    }
    myStateConditionVariable.notify_one();
}

void KC_RenderSystemProvider::Render() const
{
    KC_PROFILE_RENDER
    myRenderWindow.clear(sf::Color::Black);

    {
        KC_PROFILE_RENDERDRAW
        RunSystem<KC_DrawCircleSystem>();
    }

#if IS_IMGUI
    ImGui::SFML::Render(myRenderWindow);
#endif // IS_IMGUI
    myRenderWindow.display();
}

void KC_RenderSystemProvider::StopAndWait()
{
    {
        std::lock_guard lock { myStateMutex };
        myRenderThreadState = RenderThreadState::Stop;
    }

    myRenderThread.join();
}
