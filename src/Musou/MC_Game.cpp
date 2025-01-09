#include "MC_Game.h"

#include "MC_BounceOnBorderSystem.h"
#include "MC_MoveSystem.h"

#include "KC_Camera.h"
#include "KC_CircleRenderer.h"
#include "KC_Defines.h"
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
#if IS_IMGUI
    , myCameraZoom(nullptr)
#endif // IS_IMGUI
{
}

void MC_Game::Init()
{
    std::srand(std::time(nullptr));

    KC_EntityManager& entityManager = myWorld.GetEntityManager();
    KC_MainComponentManager& componentManager = myWorld.GetComponentManager();

    KC_Entity cameraEntity = entityManager.CreateEntity();
    componentManager.AddComponent<KC_Transform>(cameraEntity);
    KC_Camera& camera = componentManager.AddComponent<KC_Camera>(cameraEntity);
#if IS_IMGUI
    myCameraZoom = &camera.myZoom;
#else
    KC_UNUSED(camera);
#endif // IS_IMGUI

    for (int i = 0; i < 10000; ++i)
    {
        KC_Entity entity = entityManager.CreateEntity();

        KC_Transform& transform = componentManager.AddComponent<KC_Transform>(entity);
        transform.myPosition = { static_cast<float>(std::rand() % 800) - 400.f, static_cast<float>(std::rand() % 600) - 300.f };

        KC_Velocity& velocity = componentManager.AddComponent<KC_Velocity>(entity);
        velocity.myVector = sf::Transform().rotate(sf::degrees(std::rand() % 365)) * sf::Vector2f(0.f, 1.f) * 100.f;

        KC_CircleRenderer& circle = componentManager.AddComponent<KC_CircleRenderer>(entity);
        circle.myRadius = 2.f;
        circle.myFillColor = {255, 0, static_cast<std::uint8_t>(255 * i / 10000)};
    }
}

void MC_Game::Update(float anElapsedTime)
{
    myGameSystemProvider.RunSystem<MC_MoveSystem>(anElapsedTime);
    myGameSystemProvider.RunSystem<MC_BounceOnBorderSystem>();
}

#if IS_IMGUI
void MC_Game::ImGui()
{
    ImGui::Begin("Musou");
    ImGui::SliderFloat("Camera Zoom", myCameraZoom, 0.5f, 3.f);
    ImGui::End();
}
#endif // IS_IMGUI