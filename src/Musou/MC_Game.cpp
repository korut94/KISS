#include "MC_Game.h"

#include "MC_BounceOnBorderSystem.h"
#include "MC_MoveSystem.h"

#include "KC_CircleRenderer.h"
#include "KC_Transform.h"
#include "KC_World.h"

#include <SFML/Graphics/Transform.hpp>

#if IS_IMGUI
#include "imgui.h"
#endif // IS_IMGUI

#include <cstdlib>
#include <ctime>

MC_Game::MC_Game(KC_World& aWorld)
    : Super(aWorld)
    , myWorld(aWorld)
    , myGameSystemProvider(myWorld.GetComponentManager())
{
}

void MC_Game::Init()
{
    std::srand(std::time(nullptr));

    KC_EntityManager& entityManager = myWorld.GetEntityManager();
    KC_MainComponentManager& componentManager = myWorld.GetComponentManager();

    for (int i = 0; i < 5000; ++i)
    {
        KC_Entity entity = entityManager.CreateEntity();

        KC_Transform& transform = componentManager.AddComponent<KC_Transform>(entity);
        const float x = static_cast<float>(600 + std::rand() % 80);
        const float y = static_cast<float>(280 + std::rand() % 60);
        transform.myPosition = { x, y };

        KC_Velocity& velocity = componentManager.AddComponent<KC_Velocity>(entity);
        velocity.myVector = sf::Transform().rotate(sf::degrees(std::rand() % 365)) * sf::Vector2f(0.f, 1.f) * 50.f;

        KC_CircleRenderer& circle = componentManager.AddComponent<KC_CircleRenderer>(entity);
        circle.myRadius = 2.f;
        circle.myFillColor = {255, 0, static_cast<std::uint8_t>(255 * i / 10000)};
    }
}

void MC_Game::Update(float anElapsedTime)
{
    myGameSystemProvider.RunSystem<MC_MoveSystem>(anElapsedTime);
    myGameSystemProvider.RunSystem<MC_BounceOnBorderSystem>(anElapsedTime);
}

#if IS_IMGUI
void MC_Game::ImGui()
{
}
#endif // IS_IMGUI