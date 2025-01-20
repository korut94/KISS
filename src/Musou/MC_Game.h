#pragma once

#include "KC_Game.h"

class MC_Game final : public KC_Game
{
public:
    void Init(KC_World& aWorld);
    void Update(KC_GameSystemProvider& aGameSystemProvider);
    void FixedUpdate(KC_GameSystemProvider& aGameSystemProvider);
#if IS_IMGUI
    void ImGui();
#endif // IS_IMGUI
};
