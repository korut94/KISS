#pragma once

#include "KT_ImGui.h"

#include "KC_Game.h"
#include "KC_World.h"

template <typename TGame>
class KT_Editor final : public KC_Game
{
    using Super = KC_Game;

public:
    explicit KT_Editor(KC_World& aWorld);

    void Init();
    void Update();
    void ImGui();

private:
    TGame myGame;

    bool myShowProfiler = false;
    bool myShowImGuiDemo = false;
};

template <typename TGame>
KT_Editor<TGame>::KT_Editor(KC_World& aWorld)
    : Super(aWorld)
    , myGame(aWorld)
{
}

template <typename TGame>
void KT_Editor<TGame>::Init()
{
    myGame.Init();
}

template <typename TGame>
void KT_Editor<TGame>::Update()
{
    myGame.Update();
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

    if (myShowProfiler) ImGui::Editor::Profiler(&myShowProfiler);
    if (myShowImGuiDemo) ImGui::ShowDemoWindow(&myShowImGuiDemo);

    myGame.ImGui();
}
