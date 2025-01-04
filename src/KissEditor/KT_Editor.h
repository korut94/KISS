#pragma once

#include "KT_ImGui.h"

#include "KC_Game.h"

class KC_World;

template <typename TGame>
class KT_Editor final : public KC_Game
{
public:
    void Update(KC_World& aWorld);
    void ImGui();

private:
    TGame myGame;
};

template <typename TGame>
void KT_Editor<TGame>::Update(KC_World& aWorld)
{
    myGame.Update(aWorld);
}

template <typename TGame>
void KT_Editor<TGame>::ImGui()
{
    ImGui::Editor::Profiler();
    ImGui::ShowDemoWindow();
    myGame.ImGui();
}
