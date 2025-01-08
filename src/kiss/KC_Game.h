#pragma once

class KC_World;

class KC_Game
{
public:
    explicit KC_Game(KC_World& aWorld);

    void Init() {}
    void Update() {}
#if IS_IMGUI
    void ImGui() {}
#endif // IS_IMGUI

protected:
    KC_World& myWorld;
};
