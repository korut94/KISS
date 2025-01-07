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

    bool myShowProfiler = false;
    bool myShowImGuiDemo = false;
};

template <typename TGame>
void KT_Editor<TGame>::Update(KC_World& aWorld)
{
    myGame.Update(aWorld);
}

template <typename TGame>
void KT_Editor<TGame>::ImGui()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Tools"))
        {
            ImGui::MenuItem("Profiler", nullptr, &myShowProfiler);
            ImGui::MenuItem("Show ImGui Demo", nullptr, &myShowImGuiDemo);

            ImGui::EndMenu();
        }
    }
    ImGui::EndMainMenuBar();

    if (myShowProfiler) ImGui::Editor::Profiler();
    if (myShowImGuiDemo) ImGui::ShowDemoWindow();

    myGame.ImGui();
}
