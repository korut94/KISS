#include "KC_RenderThread.h"

#include "KC_CircleRenderSystem.h"

#include <SFML/Graphics.hpp>

KC_RenderThread::KC_RenderThread(sf::RenderWindow& aRenderWindow)
    : myRenderWindow(aRenderWindow)
    , myState(State::Run)
{
    myThread = std::move(std::thread(&KC_RenderThread::Run, this));
}

void KC_RenderThread::UpdateFrame(const KC_MainComponentManager& aMainComponentManager)
{
    std::unique_lock lock { myStateMutex };
    
    myState = State::UpdateFrame;
    myStateConditionVariable.wait(lock, [this]{ return myState == State::Wait; });

    aMainComponentManager.AssignComponents(myRenderSystemProvider.GetComponentManager());
    myState = State::ReadyToStart;

    lock.unlock();
    myStateConditionVariable.notify_one();

    lock.lock();
    myStateConditionVariable.wait(lock, [this]{ return myState == State::Run; });
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
        myStateConditionVariable.wait(lock, [this]{ return myState == State::ReadyToStart; });

        myState = State::Run;
    }
    myStateConditionVariable.notify_one();
}

void KC_RenderThread::Render() const
{
    myRenderWindow.clear(sf::Color::Black);
    myRenderSystemProvider.Run<KC_CircleRenderSystem>(myRenderWindow);
    myRenderWindow.display();
}
