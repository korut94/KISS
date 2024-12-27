#include "KC_GameManager.h"

#include "KC_Assert.h"
#include "KC_RenderSystemProvider.h"
#include "KC_RenderThread.h"

#include <SFML/Graphics.hpp>

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
    sf::RenderWindow window = sf::RenderWindow({1920u, 1080u}, "KISS");
    window.setActive(false);
    
    KC_World world;
    myWorld = &world;

    KC_RenderThread renderThread(window);

    for (;;)
    {
        if (!ProcessEvent(window))
            break;

        renderThread.UpdateFrame(world.GetComponentManager());
    }

    renderThread.StopAndWait();
}

bool KC_GameManager::ProcessEvent(sf::RenderWindow& aRenderWindow)
{
    for (sf::Event event; aRenderWindow.pollEvent(event);)
    {
        if (event.type == sf::Event::Closed)
        {
            return false;
        }
    }
    return true;
}
