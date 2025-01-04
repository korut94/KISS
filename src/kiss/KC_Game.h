#pragma once

class KC_World;

class KC_Game
{
public:
    void Update(KC_World& /*aWorld*/) {}
#if IS_IMGUI
    void ImGui() {}
#endif // IS_IMGUI
};
