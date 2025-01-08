#include "MC_Game.h"

#include "KC_CircleRenderer.h"
#include "KC_Transform.h"
#include "KC_World.h"

MC_Game::MC_Game(KC_World& aWorld)
    : Super(aWorld)
{
}

void MC_Game::Init()
{
    KC_EntityManager& entityManager = myWorld.GetEntityManager();
    KC_MainComponentManager& componentManager = myWorld.GetComponentManager();

    for (int i = 0; i < 10000; ++i)
    {
        KC_Entity entity = entityManager.CreateEntity();

        KC_Transform& transform = componentManager.AddComponent<KC_Transform>(entity);
        transform.myPosition = { (i % 100) * 5.f + 50.f, (i / 100) * 5.f + 50.f };

        KC_CircleRenderer& circle = componentManager.AddComponent<KC_CircleRenderer>(entity);
        circle.myRadius = 2.f;
        circle.myFillColor = {255, 0, static_cast<std::uint8_t>(255 * i / 10000)};
    }
}

void MC_Game::Update()
{
}
