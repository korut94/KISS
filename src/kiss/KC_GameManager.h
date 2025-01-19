#pragma once

#include "KC_GameManager.h"

#include "KC_Assert.h"
#include "KC_GameSystemProvider.h"
#include "KC_Profiling.h"
#include "KC_RenderSystemProvider.h"
#include "KC_ThreadManager.h"
#include "KC_Time.h"
#include "KC_World.h"

#if IS_IMGUI
#include "imgui.h"
#include "imgui-SFML.h"
#endif // IS_IMGUI

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

template <typename TGame>
class KC_GameManager final
{
public: 
    void Run();

private:
    bool ProcessEvent(sf::RenderWindow& aRenderWindow);
};

template <typename TGame>
void KC_GameManager<TGame>::Run()
{
    KC_THREAD("Main Thread")

    sf::RenderWindow window{ sf::VideoMode({ 1920u, 1080u }), "KISS" };

    KC_World world;

    TGame game;
    game.Init(world);

    KC_GameSystemProvider gameSystemProvider{ world.GetComponentManager() };
    KC_RenderSystemProvider renderSystemProvider{ window };
    bool proceed = true;

    KC_Time::SetFrameZero();
    KC_Time previousFrameTime = KC_Time::Now();

    while (proceed)
    {
        KC_PROFILE_GAME
        const KC_Time currentFrameTime = KC_Time::Now();
        gameSystemProvider.myElapsedTime = currentFrameTime - previousFrameTime;

        proceed = ProcessEvent(window); // we will close the application at the next cycle
        // Update frame
        {
            std::unique_lock lock = std::move(renderSystemProvider.UpdateFrame());
#if IS_IMGUI
            // ImGui update must be the first operation to perform during the lock of the render thread
            ImGui::SFML::Update(window, sf::microseconds(gameSystemProvider.myElapsedTime.AsMicroseconds()));
            game.ImGui();
#endif // IS_IMGUI
            {
                KC_PROFILE_GAMERENDERCOPYCOMPONENTS
                renderSystemProvider.SetComponents(world.GetComponentManager());
            }
            renderSystemProvider.Ready(lock);
        }

        {
            KC_PROFILE_GAMEUPDATE
            KC_Time remainingFixedUpdateTime = gameSystemProvider.myElapsedTime;
            while (remainingFixedUpdateTime >= gameSystemProvider.ourFixedUpdateTime)
            {
                game.FixedUpdate(gameSystemProvider);
                remainingFixedUpdateTime -= gameSystemProvider.ourFixedUpdateTime;
            }

            game.Update(gameSystemProvider);
        }

        previousFrameTime = currentFrameTime;
    }
}

template <typename TGame>
bool KC_GameManager<TGame>::ProcessEvent(sf::RenderWindow& aRenderWindow)
{
    while (const auto event = aRenderWindow.pollEvent())
    {
#if IS_IMGUI
        ImGui::SFML::ProcessEvent(aRenderWindow, *event);
#endif // IS_IMGUI

        if (event->is<sf::Event::Closed>())
        {
            return false;
        }
    }

    return true;
}
