#pragma once

#include "KC_Game.h"
#include "KC_GameSystemProvider.h"

class MC_Game final : public KC_Game
{
    using Super = KC_Game;

public:
    explicit MC_Game(KC_World& aWorld);

    void Init();
    void Update(float anElapsedTime);
#if IS_IMGUI
    void ImGui();
#endif // IS_IMGUI

private:
    KC_World& myWorld;
    KC_GameSystemProvider myGameSystemProvider;

#if IS_IMGUI
    float* myCameraZoom;
#endif // IS_IMGUI
};
