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

    for (int i = 0; i < 1; ++i)
    {
        KC_Entity entity = entityManager.CreateEntity();

        componentManager.AddComponent<KC_Canvas>(entity);

        KC_Transform& transform = componentManager.AddComponent<KC_Transform>(entity);
        transform.myPosition = { 2.5f, 0.f };
#if IS_IMGUI
        myPosition = &transform.myPosition;
#endif // IS_IMGUI

        KC_RectCollider& collider = componentManager.AddComponent<KC_RectCollider>(entity);
        collider.myBound.SetPosition({ -1.f, -1.f });
        collider.myBound.SetSize({ 2.f, 2.f });
        KC_RectColliderPalette& colliderPalette = componentManager.AddComponent<KC_RectColliderPalette>(entity);
        colliderPalette.myColor = sf::Color::Blue;
        /*
        KC_Velocity& velocity = componentManager.AddComponent<KC_Velocity>(entity);
        velocity.myVector = sf::Vector2f({ 1.f, 0.f }).normalized() * 2.f;
        */
    }
    /*
    for (int i = 0; i < 1; ++i)
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
        velocity.myVector = sf::Transform().rotate(sf::degrees(std::rand() % 365)) * sf::Vector2f(0.f, 1.f) * 5.f;

        KC_CircleRenderer& circle = componentManager.AddComponent<KC_CircleRenderer>(entity);
        circle.myRadius = 1.f;
        circle.myFillColor = {255, 0, static_cast<std::uint8_t>(255 * i / 10000)};
    }
    */
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

    float position[2] = {myPosition->x, myPosition->y}; 
    ImGui::SliderFloat2("Position:", position, -30.f, 30.f);
    *myPosition = sf::Vector2f(position[0], position[1]);

#if IS_DEBUG_BUILD
    if (mySpatialGrid)
    {
        ImGui::SeparatorText("Spatial Grid");
        sf::Vector2i c = static_cast<sf::Vector2i>(*myPosition) / mySpatialGrid->GetGridCellSize();
        ImGui::Text("Coordinate (%d, %d)", c.x, c.y);
        ImGui::Text("%d", c.x % mySpatialGrid->GetGridCellSize());

        std::vector<std::int32_t> spatialGridIndexs;
        mySpatialGrid->GetIndexs(spatialGridIndexs);

        std::sort(spatialGridIndexs.begin(), spatialGridIndexs.end());

        const std::int32_t gridCellSize = mySpatialGrid->GetGridCellSize();

        ImGui::Text("Cells Count: %d [%dx%d]", spatialGridIndexs.size(), gridCellSize, gridCellSize);
        ImGui::Text("Cells:");
        if (spatialGridIndexs.size() > 0)
        {
            std::int32_t prevY = mySpatialGrid->GetGridCoordinate(spatialGridIndexs[0]).y;

            for (std::int32_t spatialGridIndex : spatialGridIndexs)
            {
                const sf::Vector2i gridCoordinate = mySpatialGrid->GetGridCoordinate(spatialGridIndex);
                ImGui::Text("[%d, %d]", gridCoordinate.x, gridCoordinate.y);

                if (prevY == gridCoordinate.y)
                {
                    ImGui::SameLine();
                }

                prevY = gridCoordinate.y;
            }
        }
    }
#endif // IS_DEBUG_BUILD

    ImGui::End();
}
#endif // IS_IMGUI
