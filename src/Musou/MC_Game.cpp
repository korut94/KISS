#include "MC_Game.h"

#include "MC_BounceOnCollisionSystem.h"
#include "MC_MoveSystem.h"

#include "KC_Camera.h"
#include "KC_Canvas.h"
#include "KC_ClearCanvasSystem.h"
#include "KC_Defines.h"
#include "KC_GameSystemProvider.h"
#include "KC_InitializeSpatialGridSystem.h"
#include "KC_Math.h"
#include "KC_PaintRectColliderSystem.h"
#include "KC_PaintSpatialGridSystem.h"
#include "KC_RectCollider.h"
#include "KC_RectColliderPalette.h"
#include "KC_ResolveCollisionSystem.h"
#include "KC_SpatialGridPalette.h"
#include "KC_Transform.h"
#include "KC_World.h"

#include <SFML/Graphics/Transform.hpp>

#if IS_IMGUI
#include "imgui.h"
#endif // IS_IMGUI

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>

namespace MC_Game_Private
{
#if IS_IMGUI
    float* locCameraZoom = nullptr;
    KC_SpatialGrid* locSpatialGrid = nullptr;
    std::vector<KC_CollisionEvent>* locCollisionEvents = nullptr;
#endif // IS_IMGUI
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
    MC_Game_Private::locCameraZoom = &camera.myZoom;
#else
    KC_UNUSED(camera);
#endif // IS_IMGUI

    KC_SpatialGrid& spatialGrid = aWorld.AddSpatialGrid(5.f);
#if IS_IMGUI
    MC_Game_Private::locSpatialGrid = &spatialGrid;
#else
    KC_UNUSED(spatialGrid);
#endif // IS_IMGUI

    KC_Entity spatialGridEntity = entityManager.CreateEntity();
    componentManager.AddComponent<KC_Canvas>(spatialGridEntity);
    KC_SpatialGridPalette& spatialGridPalette = componentManager.AddComponent<KC_SpatialGridPalette>(spatialGridEntity);
    spatialGridPalette.myGridCellFillColor = {0, 0, 255, 80};
    spatialGridPalette.myGridLineColor = sf::Color::White;
    spatialGridPalette.myGridLineThickness = 0.1f;

    for (int i = 0; i < 10; ++i)
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
        colliderPalette.myFillColor = {255, 0, static_cast<std::uint8_t>(255 * i / 10000)};
        colliderPalette.myOutlineThickness = 0.1f;
        colliderPalette.myOutlineColor = {255, 127, 127};

        KC_Velocity& velocity = componentManager.AddComponent<KC_Velocity>(entity);
        velocity.myVector = sf::Transform().rotate(sf::degrees(std::rand() % 365)) * sf::Vector2f(0.f, 1.f) * 20.f;
    }

    for (int i = 0; i < 4; ++i)
    {
        KC_Entity border = entityManager.CreateEntity();
        componentManager.AddComponent<KC_Canvas>(border);

        const sf::Transform rotation = sf::Transform().rotate(sf::degrees(i * 90));

        KC_Transform& transform = componentManager.AddComponent<KC_Transform>(border);
        transform.myPosition = rotation * sf::Vector2f(100.f, 0.f);

        const sf::Vector2f size = { 5.f, 200.f };
        const sf::Vector2f rotatedSize = rotation * size;
        const sf::Vector2f rectSize = { abs(rotatedSize.x), abs(rotatedSize.y) };

        KC_RectCollider& collider = componentManager.AddComponent<KC_RectCollider>(border);
        collider.myBound.SetPosition(rectSize * -0.5f);
        collider.myBound.SetSize(rectSize);
        KC_RectColliderPalette& colliderPalette = componentManager.AddComponent<KC_RectColliderPalette>(border);
        colliderPalette.myFillColor = sf::Color::Transparent;
        colliderPalette.myOutlineColor = sf::Color::Green;
        colliderPalette.myOutlineThickness = 0.2f;
    }
}

void MC_Game::Update(KC_GameSystemProvider& aGameSystemProvider)
{
    aGameSystemProvider.RunSystem<MC_BounceOnCollisionSystem>(aGameSystemProvider.myCollisionEvents);

    aGameSystemProvider.RunSystem<KC_ClearCanvasSystem>(); // Maybe move it in engine and create a function game.Draw()?
    aGameSystemProvider.RunSystem<KC_PaintSpatialGridSystem>(aGameSystemProvider.GetSpatialGrids());
    aGameSystemProvider.RunSystem<KC_PaintRectColliderSystem>();
}

void MC_Game::FixedUpdate(KC_GameSystemProvider& aGameSystemProvider)
{
    const float fixedUpdateTime = KC_GameSystemProvider::GetFixedUpdateTime();

    std::vector<KC_SpatialGrid>& spatialGrids = aGameSystemProvider.GetSpatialGrids();
    KC_ASSERT(spatialGrids.size() > 0);

    aGameSystemProvider.RunSystem<MC_MoveSystem>(fixedUpdateTime);
    aGameSystemProvider.RunSystem<KC_InitializeSpatialGridSystem>(spatialGrids);
    aGameSystemProvider.RunSystem<KC_ResolveCollisionSystem>(spatialGrids[0], aGameSystemProvider.myCollisionEvents);

#if IS_IMGUI
    MC_Game_Private::locCollisionEvents = &aGameSystemProvider.myCollisionEvents;
#endif // IS_IMGUI
}

#if IS_IMGUI
void MC_Game::ImGui()
{
    namespace Private = MC_Game_Private;

    ImGui::Begin("Musou");
    ImGui::SliderFloat("Camera Zoom", Private::locCameraZoom, 0.5f, 3.f);

    if (Private::locSpatialGrid)
    { 
        KC_ImGui(*Private::locSpatialGrid);
    }

    if (Private::locCollisionEvents)
    {
        ImGui::Text("Collision Events");
        for (const KC_CollisionEvent& collision : *Private::locCollisionEvents)
        {
            ImGui::Text("%d -> %d", collision.myEntity, collision.myOtherEntity);
        }
    }

    ImGui::End();
}
#endif // IS_IMGUI
