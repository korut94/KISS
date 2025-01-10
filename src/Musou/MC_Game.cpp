#include "MC_Game.h"

#include "MC_BounceOnBorderSystem.h"
#include "MC_MoveSystem.h"

#include "KC_Camera.h"
#include "KC_Canvas.h"
#include "KC_CircleRenderer.h"
#include "KC_ClearCanvasSystem.h"
#include "KC_Defines.h"
#include "KC_InitializeSpatialGridSystem.h"
#include "KC_GameSystemProvider.h"
#include "KC_PaintSpatialGridSystem.h"
#include "KC_RectCollider.h"
#include "KC_SpatialGridPalette.h"
#include "KC_Transform.h"
#include "KC_World.h"

#include <SFML/Graphics/Transform.hpp>

#if IS_IMGUI
#include "imgui.h"
#endif // IS_IMGUI

#include <cstdlib>
#include <ctime>

MC_Game::MC_Game()
    : mySpatialGrid(nullptr)
#if IS_IMGUI
    , myCameraZoom(nullptr)
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
    KC_SpatialGridPalette& spatialGridPalette = componentManager.AddComponent<KC_SpatialGridPalette>(spatialGrid);
    spatialGridPalette.mySpatialGrid = mySpatialGrid;

    for (int i = 0; i < 100; ++i)
    {
        KC_Entity entity = entityManager.CreateEntity();

        KC_Transform& transform = componentManager.AddComponent<KC_Transform>(entity);
        const float randomX = (static_cast<float>(std::rand()) / RAND_MAX) * 10.f;
        const float randomY = (static_cast<float>(std::rand()) / RAND_MAX) * 10.f;
        transform.myPosition = { randomX, randomY };

        KC_RectCollider& collider = componentManager.AddComponent<KC_RectCollider>(entity);
        collider.myBound.SetPosition(sf::Vector2f(-0.5f, -0.5f));
        collider.myBound.SetSize(sf::Vector2f(1.f, 1.f));

        KC_Velocity& velocity = componentManager.AddComponent<KC_Velocity>(entity);
        velocity.myVector = sf::Transform().rotate(sf::degrees(std::rand() % 365)) * sf::Vector2f(0.f, 1.f) * 10.f;

        KC_CircleRenderer& circle = componentManager.AddComponent<KC_CircleRenderer>(entity);
        circle.myRadius = 1.f;
        circle.myFillColor = {255, 0, static_cast<std::uint8_t>(255 * i / 10000)};
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
}

#if IS_IMGUI
void MC_Game::ImGui()
{
    ImGui::Begin("Musou");
    ImGui::SliderFloat("Camera Zoom", myCameraZoom, 0.5f, 3.f);

#if IS_DEBUG_BUILD
    if (mySpatialGrid)
    {
        ImGui::SeparatorText("Spatial Grid");
        auto gridCells = mySpatialGrid->GetGridCells();
        std::int32_t newLine = 0;

        ImGui::Text("Cells Count: %d [%dx%d]", gridCells.size(), mySpatialGrid->GetGridCellSize(), mySpatialGrid->GetGridCellSize());
        ImGui::Text("Cells:");
        for (auto [index, entitySet] : gridCells)
        {
            ImGui::Text("[%d, %d]", index, entitySet.Count());
            if (++newLine % 4 != 3)
            {
                ImGui::SameLine();
            }
        }
    }
#endif // IS_DEBUG_BUILD

    ImGui::End();
}
#endif // IS_IMGUI
