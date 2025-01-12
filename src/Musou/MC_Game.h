#pragma once

#include "KC_Game.h"

#include <SFML/System/Vector2.hpp>

class KC_SpatialGrid;

class MC_Game final : public KC_Game
{
public:
    MC_Game();

    void Init(KC_World& aWorld);
    void Update(KC_GameSystemProvider& aGameSystemProvider);
#if IS_IMGUI
    void ImGui();
#endif // IS_IMGUI

private:
    KC_SpatialGrid* mySpatialGrid;

#if IS_IMGUI
    float* myCameraZoom;
#endif // IS_IMGUI
};
