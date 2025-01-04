#include "MC_Game.h"

#include "KC_CircleRenderer.h"
#include "KC_Transform.h"
#include "KC_World.h"

#if IS_IMGUI
#include "imgui.h"
#endif // IS_IMGUI

MC_Game::MC_Game()
    : myIsInitialized(false)
{
}

void MC_Game::Update(KC_World& aWorld)
{
    if (!myIsInitialized)
    {
        KC_EntityManager& entityManager = aWorld.GetEntityManager();
        KC_MainComponentManager& componentManager = aWorld.GetComponentManager();

        for (int i = 0; i < 1000; ++i)
        {
            KC_Entity entity = entityManager.CreateEntity();

            KC_Transform& transform = componentManager.AddComponent<KC_Transform>(entity);
            transform.myPosition = { (i % 50) * 15.f + 150.f, (i / 50) * 15.f + 150.f };

            KC_CircleRenderer& circle = componentManager.AddComponent<KC_CircleRenderer>(entity);
            circle.myRadius = 10.f;
            circle.myFillColor = sf::Color::Green;
        }

        myIsInitialized = true;
    }
}
