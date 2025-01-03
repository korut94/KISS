#pragma once

#include "KC_Game.h"

class MC_Game final : public KC_Game
{
public:
    void Update();
#if IS_IMGUI
    void ImGui();
#endif // IS_IMGUI
};
