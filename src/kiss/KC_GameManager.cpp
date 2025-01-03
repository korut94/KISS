#include "KC_GameManager.h"

#include "KC_Assert.h"
#include "KC_RenderSystemProvider.h"
#include "KC_RenderThread.h"

#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

KC_GameManager::KC_GameManager()
    : myWorld(nullptr) 
{
}

const KC_World &KC_GameManager::GetWorld() const
{
    KC_ASSERT(myWorld);
    return *myWorld;
}

KC_World &KC_GameManager::GetWorld()
{
    KC_ASSERT(myWorld);
    return *myWorld;
}

void KC_GameManager::Run()
{
    sf::RenderWindow window = sf::RenderWindow({1280u, 720u}, "KISS");
    window.setActive(false);

    KC_World world;
    myWorld = &world;

    sf::Clock clock;
    bool proceed = true;

    KC_RenderThread renderThread(window);
    
    while (proceed)
    {
        const sf::Time elapsedTime = clock.restart();
        proceed = ProcessEvent(window); // we will close the application on the next cycle
        // Update frame
        {
            std::unique_lock lock = std::move(renderThread.UpdateFrame());
            renderThread.SetComponents(world.GetComponentManager());
            renderThread.ImGuiUpdate(elapsedTime);
            renderThread.Ready(lock);
        }
    }
}

bool KC_GameManager::ProcessEvent(sf::RenderWindow& aRenderWindow)
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
