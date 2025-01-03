#include "KC_RenderSystemProvider.h"

#include "KC_Assert.h"
#include "KC_CircleRenderSystem.h"

#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics.hpp>

KC_RenderSystemProvider::KC_RenderSystemProvider(sf::RenderWindow& aRenderWindow)
    : myRenderWindow(aRenderWindow)
    , myRenderThreadState(RenderThreadState::Run)
{
    ImGuiInit();
    myRenderThread = std::move(std::thread(&KC_RenderSystemProvider::RenderThread, this));
}

KC_RenderSystemProvider::~KC_RenderSystemProvider()
{
    StopAndWait();
    ImGui::SFML::Shutdown();
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

void KC_RenderSystemProvider::ImGuiUpdate(sf::Time elapsedTime)
{
    ImGui::SFML::Update(myRenderWindow, elapsedTime);
    ImGui::ShowDemoWindow();
}

void KC_RenderSystemProvider::Ready(std::unique_lock<std::mutex>& aLock)
{
    KC_ASSERT(aLock.owns_lock());
    myRenderThreadState = RenderThreadState::Ready;

    aLock.unlock();
    myStateConditionVariable.notify_one();

    aLock.lock();
    myStateConditionVariable.wait(aLock, [this]{ return myRenderThreadState == RenderThreadState::Run; });
}

void KC_RenderSystemProvider::ImGuiInit()
{
    KC_ASSERT(ImGui::SFML::Init(myRenderWindow));
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}

void KC_RenderSystemProvider::RenderThread()
{
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
    myRenderWindow.clear(sf::Color::Black);

    RunSystem<KC_CircleRenderSystem>();
    
    ImGui::SFML::Render(myRenderWindow);
    
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
