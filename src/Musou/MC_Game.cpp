#include "MC_Game.h"

#include "MC_BounceOnBorderSystem.h"
#include "MC_MoveSystem.h"

#include "KC_Camera.h"
#include "KC_Canvas.h"
#include "KC_ClearCanvasSystem.h"
#include "KC_Defines.h"
#include "KC_InitializeSpatialGridSystem.h"
#include "KC_GameSystemProvider.h"
#include "KC_PaintRectColliderSystem.h"
#include "KC_PaintSpatialGridSystem.h"
#include "KC_RectCollider.h"
#include "KC_RectColliderPalette.h"
#include "KC_SpatialGridPalette.h"
#include "KC_Transform.h"
#include "KC_World.h"

#include <SFML/Graphics/Transform.hpp>

#if IS_IMGUI
#include "imgui.h"

#include <algorithm>
#endif // IS_IMGUI
#include <cstdlib>
#include <ctime>

MC_Game::MC_Game()
    : mySpatialGrid(nullptr)
#if IS_IMGUI
    , myCameraZoom(nullptr)
    , myPosition(nullptr)
#endif // IS_IMGUI
{
}

void MC_Game::Init(KC_World& aWorld)
{
    std::srand(std::time(nullptr));

    KC_EntityManager& entityManager = aWorld.GetEntityManager();
    KC_MainComponentManager& componentManager = aWorld.GetComponentManager();

    KC_Entity cameraEntity = entityManager.CreateEntity();
    componentManager.AddComponent<KC_Transform>(cameraEntity);
    KC_Camera& camera = componentManager.AddComponent<KC_Camera>(cameraEntity);
#if IS_IMGUI
    myCameraZoom = &camera.myZoom;
#else
    KC_UNUSED(camera);
#endif // IS_IMGUI

    mySpatialGrid = &aWorld.AddSpatialGrid(5);

    KC_Entity spatialGrid = entityManager.CreateEntity();
    componentManager.AddComponent<KC_Canvas>(spatialGrid);
    componentManager.AddComponent<KC_SpatialGridPalette>(spatialGrid, *mySpatialGrid);

    for (int i = 0; i < 10000; ++i)
    {
        KC_Entity entity = entityManager.CreateEntity();
        componentManager.AddComponent<KC_Canvas>(entity);

        KC_Transform& transform = componentManager.AddComponent<KC_Transform>(entity);
        const float rX = static_cast<float>(std::rand()) / RAND_MAX;
        const float rY = static_cast<float>(std::rand()) / RAND_MAX;
        const float randomX =  100.f * rX - 50.f;
        const float randomY = 100.f * rY - 50.f;
        transform.myPosition = { randomX, randomY };

        KC_RectCollider& collider = componentManager.AddComponent<KC_RectCollider>(entity);
        collider.myBound.SetPosition(sf::Vector2f(-0.5f, -0.5f));
        collider.myBound.SetSize(sf::Vector2f(1.f, 1.f));
        KC_RectColliderPalette& colliderPalette = componentManager.AddComponent<KC_RectColliderPalette>(entity);
        colliderPalette.myColor = {255, 0, static_cast<std::uint8_t>(255 * i / 10000)};

        KC_Velocity& velocity = componentManager.AddComponent<KC_Velocity>(entity);
        velocity.myVector = sf::Transform().rotate(sf::degrees(std::rand() % 365)) * sf::Vector2f(0.f, 1.f) * 20.f;
    }
}

void MC_Game::Update(KC_GameSystemProvider& aGameSystemProvider)
{
    KC_ASSERT(mySpatialGrid != nullptr);

    const float elapsedTime = aGameSystemProvider.GetElapsedTime();

    aGameSystemProvider.RunSystem<MC_MoveSystem>(elapsedTime);
    aGameSystemProvider.RunSystem<KC_InitializeSpatialGridSystem>(*mySpatialGrid);
    aGameSystemProvider.RunSystem<MC_BounceOnBorderSystem>();

    aGameSystemProvider.RunSystem<KC_ClearCanvasSystem>(); // Maybe move it in engine and create a function game.Draw()?
    aGameSystemProvider.RunSystem<KC_PaintSpatialGridSystem>();
    aGameSystemProvider.RunSystem<KC_PaintRectColliderSystem>();
}

#if IS_IMGUI
void MC_Game::ImGui()
{
    ImGui::Begin("Musou");
    ImGui::SliderFloat("Camera Zoom", myCameraZoom, 0.5f, 3.f);

    if (myPosition)
    {
        float position[2] = {myPosition->x, myPosition->y}; 
        ImGui::SliderFloat2("Position:", position, -30.f, 30.f);
        *myPosition = sf::Vector2f(position[0], position[1]);
    }

    if (mySpatialGrid)
    { 
        KC_ImGui(*mySpatialGrid);
    }

    ImGui::End();
}
#endif // IS_IMGUI
