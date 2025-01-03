#pragma once

#include "KC_GameManager.h"

#include "KC_Assert.h"
#include "KC_RenderSystemProvider.h"
#include "KC_World.h"

#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

template <typename TGame>
class KC_GameManager final
{
public:
    KC_GameManager();

    const KC_World& GetWorld() const;
    KC_World& GetWorld();

    void Run();

private:
    bool ProcessEvent(sf::RenderWindow& aRenderWindow);

    KC_World* myWorld;
};

template <typename TGame>
KC_GameManager<TGame>::KC_GameManager()
    : myWorld(nullptr) 
{
}

template <typename TGame>
const KC_World& KC_GameManager<TGame>::GetWorld() const
{
    KC_ASSERT(myWorld);
    return *myWorld;
}

template <typename TGame>
KC_World& KC_GameManager<TGame>::GetWorld()
{
    KC_ASSERT(myWorld);
    return *myWorld;
}

template <typename TGame>
void KC_GameManager<TGame>::Run()
{
    sf::RenderWindow window = sf::RenderWindow({1280u, 720u}, "KISS");
    window.setActive(false);

    KC_World world;
    myWorld = &world;

    sf::Clock clock;
    bool proceed = true;

    TGame game;
    KC_RenderSystemProvider renderSystemProvider(window);

    while (proceed)
    {
        const sf::Time elapsedTime = clock.restart();
        proceed = ProcessEvent(window); // we will close the application on the next cycle
        // Update frame
        {
            std::unique_lock lock = std::move(renderSystemProvider.UpdateFrame());
            renderSystemProvider.SetComponents(world.GetComponentManager());
            renderSystemProvider.ImGuiUpdate(elapsedTime);
            game.ImGui();
            renderSystemProvider.Ready(lock);
        }

        game.Update();
    }
}

template <typename TGame>
bool KC_GameManager<TGame>::ProcessEvent(sf::RenderWindow& aRenderWindow)
{
    for (sf::Event event; aRenderWindow.pollEvent(event);)
    {
        ImGui::SFML::ProcessEvent(aRenderWindow, event);

        if (event.type == sf::Event::Closed)
        {
            return false;
        }
    }

    return true;
}
