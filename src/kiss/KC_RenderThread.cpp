#include "KC_RenderThread.h"

#include "KC_Assert.h"
#include "KC_CircleRenderSystem.h"

#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics.hpp>

KC_RenderThread::KC_RenderThread(sf::RenderWindow& aRenderWindow)
    : myRenderWindow(aRenderWindow)
    , myState(State::Run)
{
    ImGuiInit();

    myThread = std::move(std::thread(&KC_RenderThread::Run, this));
}

KC_RenderThread::~KC_RenderThread()
{
    StopAndWait();
    ImGui::SFML::Shutdown();
}

std::unique_lock<std::mutex> KC_RenderThread::UpdateFrame()
{
    std::unique_lock lock { myStateMutex };
    
    myState = State::UpdateFrame;
    myStateConditionVariable.wait(lock, [this]{ return myState == State::Wait; });

    return lock;
}

void KC_RenderThread::SetComponents(const KC_MainComponentManager& aMainComponentManager)
{
    aMainComponentManager.AssignComponents(myRenderSystemProvider.GetComponentManager());
}

void KC_RenderThread::ImGuiUpdate(sf::Time elapsedTime)
{
    ImGui::SFML::Update(myRenderWindow, elapsedTime);
    ImGui::ShowDemoWindow();
}

void KC_RenderThread::Ready(std::unique_lock<std::mutex>& aLock)
{
    KC_ASSERT(aLock.owns_lock());
    myState = State::Ready;

    aLock.unlock();
    myStateConditionVariable.notify_one();

    aLock.lock();
    myStateConditionVariable.wait(aLock, [this]{ return myState == State::Run; });
}

void KC_RenderThread::ImGuiInit()
{
    KC_ASSERT(ImGui::SFML::Init(myRenderWindow));
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}

void KC_RenderThread::StopAndWait()
{
    {
        std::lock_guard lock { myStateMutex };
        myState = State::Stop;
    }

    myThread.join();
}

void KC_RenderThread::Run()
{
    myRenderWindow.setActive(true);

    while (myRenderWindow.isOpen())
    {
        switch (GetState())
        {
        case State::UpdateFrame: Wait();
        case State::Run: Render(); break;
        case State::Stop: myRenderWindow.close(); break;
        }
    }
}

KC_RenderThread::State KC_RenderThread::GetState()
{
    std::lock_guard lock { myStateMutex };
    return myState;
}

void KC_RenderThread::Wait()
{
    {
        std::lock_guard lock { myStateMutex };
        myState = State::Wait;
    }
    myStateConditionVariable.notify_one();

    {
        std::unique_lock lock { myStateMutex };
        myStateConditionVariable.wait(lock, [this]{ return myState == State::Ready; });

        myState = State::Run;
    }
    myStateConditionVariable.notify_one();
}

void KC_RenderThread::Render() const
{
    myRenderWindow.clear(sf::Color::Black);
    myRenderSystemProvider.Run<KC_CircleRenderSystem>(myRenderWindow);
    ImGui::SFML::Render(myRenderWindow);
    myRenderWindow.display();
}
