#pragma once

#include "KT_ImGui.h"

#include "KC_Game.h"

template <typename TGame>
class KT_Editor final : public KC_Game
{
public:
    void Init(KC_World& aWorld);
    void Update(KC_GameSystemProvider& aGameSystemProvider);
    void FixedUpdate(KC_GameSystemProvider& aGameSystemProvider);
    void ImGui();

private:
    TGame myGame;

    bool myShowProfiler = false;
    bool myShowImGuiDemo = false;
};

template <typename TGame>
void KT_Editor<TGame>::Init(KC_World& aWorld)
{
    myGame.Init(aWorld);
}

template <typename TGame>
void KT_Editor<TGame>::Update(KC_GameSystemProvider& aGameSystemProvider)
{
    myGame.Update(aGameSystemProvider);
}

template <typename TGame>
void KT_Editor<TGame>::FixedUpdate(KC_GameSystemProvider& aGameSystemProvider)
{
    myGame.FixedUpdate(aGameSystemProvider);
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
