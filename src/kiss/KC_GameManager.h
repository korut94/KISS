#pragma once

#include "KC_GameManager.h"

#include "KC_Assert.h"
#include "KC_Profiler.h"
#include "KC_Profiling.h"
#include "KC_RenderSystemProvider.h"
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
    KC_Profiler profiler;
#endif // IS_DEBUG_BUILD

    sf::RenderWindow window = sf::RenderWindow({1280u, 720u}, "KISS");
    window.setActive(false);

    sf::Clock clock;
    bool proceed = true;

    KC_World world;

    TGame game;
    KC_RenderSystemProvider renderSystemProvider(window);

    while (proceed)
    {
        const sf::Time elapsedTime = clock.restart();
        proceed = ProcessEvent(window); // we will close the application at the next cycle
        // Update frame
        {
            KC_PROFILE_GAMERENDERSYNC;
            std::unique_lock lock = std::move(renderSystemProvider.UpdateFrame());
            renderSystemProvider.SetComponents(world.GetComponentManager());
#if IS_IMGUI
            renderSystemProvider.ImGuiUpdate(elapsedTime);
            game.ImGui();
#endif // IS_IMGUI
            renderSystemProvider.Ready(lock);
        }

        KC_PROFILE_GAME;
        game.Update(world);
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
