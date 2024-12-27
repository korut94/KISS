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
    
    KC_ASSERT(ImGui::SFML::Init(window));
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    
    KC_World world;
    myWorld = &world;

    KC_RenderThread renderThread(window);

    sf::Clock clock;
    for (;;)
    {
        if (!ProcessEvent(window))
            break;

        renderThread.UpdateFrame(world.GetComponentManager(), clock.restart());
    }

    renderThread.StopAndWait();
    ImGui::SFML::Shutdown();
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
