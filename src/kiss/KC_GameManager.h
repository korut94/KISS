#pragma once

#include "KC_GameManager.h"

#include "KC_Assert.h"
#include "KC_ProfileManager.h"
#include "KC_Profiling.h"
#include "KC_RenderSystemProvider.h"
#include "KC_Time.h"
#include "KC_World.h"

#if IS_IMGUI
#include "imgui.h"
#include "imgui-SFML.h"
#endif // IS_IMGUI

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
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
#if IS_DEBUG_BUILD
    KC_ProfileManager profileManager;
#endif // IS_DEBUG_BUILD

    sf::RenderWindow window = sf::RenderWindow({1280u, 720u}, "KISS");
    window.setActive(false);

    bool proceed = true;

    KC_World world;

    TGame game;
    KC_RenderSystemProvider renderSystemProvider(window);

    KC_Time::SetFrameZero();
    KC_Time previousFrameTime = KC_Time::Now();

    while (proceed)
    {
        KC_PROFILE_GAME
        const KC_Time currentFrameTime = KC_Time::Now();
        const KC_Time elapsedTime = currentFrameTime - previousFrameTime;

        proceed = ProcessEvent(window); // we will close the application at the next cycle
        // Update frame
        {
            std::unique_lock lock = std::move(renderSystemProvider.UpdateFrame());
            {
                KC_PROFILE_GAMERENDERSERVICEPROVIDER
                renderSystemProvider.SetComponents(world.GetComponentManager());
            }
#if IS_IMGUI
            renderSystemProvider.ImGuiUpdate(sf::microseconds(elapsedTime.AsMicroseconds()));
            game.ImGui();
#endif // IS_IMGUI
            renderSystemProvider.Ready(lock);
        }

        {
            KC_PROFILE_GAMEUPDATE
            game.Update(world);
        }

        previousFrameTime = currentFrameTime;
    }
}

template <typename TGame>
bool KC_GameManager<TGame>::ProcessEvent(sf::RenderWindow& aRenderWindow)
{
    for (sf::Event event; aRenderWindow.pollEvent(event);)
    {
#if IS_IMGUI
        ImGui::SFML::ProcessEvent(aRenderWindow, event);
#endif // IS_IMGUI

        if (event.type == sf::Event::Closed)
        {
            return false;
        }
    }

    return true;
}
