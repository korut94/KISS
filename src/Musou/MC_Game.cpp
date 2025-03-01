#include "MC_Game.h"

#include "MC_BounceOnCollisionSystem.h"
#include "MC_MoveSystem.h"

#include "KC_Camera.h"
#include "KC_Canvas.h"
#include "KC_ClearCanvasSystem.h"
#include "KC_Defines.h"
#include "KC_DetectCollisionSystem.h"
#include "KC_GameSystemProvider.h"
#include "KC_Math.h"
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
#endif // IS_IMGUI

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>

namespace MC_Game_Private
{
#if IS_IMGUI
    float* locCameraZoom = nullptr;
    const KC_SpatialGrid* locSpatialGrid = nullptr;
    const KC_CollisionEventSet* locCollisionEventSet = nullptr;
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

    for (int i = 0; i < 100; ++i)
    {
        KC_Entity entity = entityManager.CreateEntity();
        componentManager.AddComponent<KC_Canvas>(entity);

        KC_Transform& transform = componentManager.AddComponent<KC_Transform>(entity);
        const float x = -50.f + (i % 10) * 2;
        const float y = -50.f + (i / 10) * 2;
        transform.myPosition = { x, y };

        KC_RectCollider& collider = componentManager.AddComponent<KC_RectCollider>(entity);
        collider.myBound.SetPosition(sf::Vector2f(-0.5f, -0.5f));
        collider.myBound.SetSize(sf::Vector2f(1.f, 1.f));
        KC_RectColliderPalette& colliderPalette = componentManager.AddComponent<KC_RectColliderPalette>(entity);
        colliderPalette.myFillColor = {255, 0, static_cast<std::uint8_t>(255 * i / 10000)};
        colliderPalette.myOutlineThickness = 0.1f;
        colliderPalette.myOutlineColor = {255, 127, 127};

        KC_Velocity& velocity = componentManager.AddComponent<KC_Velocity>(entity);
        velocity.myVector = sf::Transform().rotate(sf::degrees(std::rand() % 365)) * sf::Vector2f(0.f, 1.f) * 10.f;
        
        spatialGrid.InsertEntity(entity, transform * collider.myBound);
    }

    for (int i = 0; i < 4; ++i)
    {
        KC_Entity border = entityManager.CreateEntity();
        componentManager.AddComponent<KC_Canvas>(border);

        const sf::Transform rotation = sf::Transform().rotate(sf::degrees(i * 90));

        KC_Transform& transform = componentManager.AddComponent<KC_Transform>(border);
        transform.myPosition = rotation * sf::Vector2f(100.f, 0.f);

        componentManager.AddComponent<KC_Velocity>(border);

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

        spatialGrid.InsertEntity(border, transform * collider.myBound);
    }
}

void MC_Game::Update(KC_GameSystemProvider& aGameSystemProvider)
{
    aGameSystemProvider.RunSystem<KC_DetectCollisionSystem>();
    aGameSystemProvider.RunSystem<MC_MoveSystem>();
    aGameSystemProvider.RunSystem<MC_BounceOnCollisionSystem>();

    aGameSystemProvider.RunSystem<KC_ClearCanvasSystem>(); // Maybe move it in engine and create a function game.Draw()?
    aGameSystemProvider.RunSystem<KC_PaintSpatialGridSystem>();
    aGameSystemProvider.RunSystem<KC_PaintRectColliderSystem>();

#if IS_IMGUI
    MC_Game_Private::locCollisionEventSet = &aGameSystemProvider.GetCollisionEventSet();
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
        ImGui::SeparatorText("Spatial Grid");
        KC_ImGui(*Private::locSpatialGrid);
    }

    if (Private::locCollisionEventSet)
    {
        ImGui::SeparatorText("Collision Events");
        KC_ImGui(*Private::locCollisionEventSet);
    }

    ImGui::End();
}
#endif // IS_IMGUI
