#include "Profiler.h"

#include "KC_Profiler.h"

#include "imgui.h"

namespace ImGui
{
namespace Editor
{
void Profiler()
{
    KC_Profiler& profiler = KC_Profiler::GetProfiler();

    ImGui::Begin("Profiler");
    ImGui::Text("FPS: %d", static_cast<int>(1.f / profiler.GetTime(KC_ProfileTimerType::Rendering).asSeconds()));

    if (ImGui::CollapsingHeader("Game Thread"))
    {
        ImGui::Text("Game: %dms", profiler.GetTime(KC_ProfileTimerType::Game).asMilliseconds());
        ImGui::Text("Game-Render Sync: %dms", profiler.GetTime(KC_ProfileTimerType::GameRenderSync).asMilliseconds());
    }
    if (ImGui::CollapsingHeader("Render Thread"))
    {
        ImGui::Text("Render: %dms", profiler.GetTime(KC_ProfileTimerType::Rendering).asMilliseconds());
        ImGui::Text("Display: %dms", profiler.GetTime(KC_ProfileTimerType::RenderDisplay).asMilliseconds());
        
        ImGui::Text("Draw: %dms", profiler.GetTime(KC_ProfileTimerType::RenderDraw).asMilliseconds());
        if (ImGui::TreeNode("CircleRenderSystem"))
        {
            ImGui::Text("Run: %dms", profiler.GetTime(KC_ProfileTimerType::RenderSystemRun).asMilliseconds());
            ImGui::Text("Get EntitySet: %dus", profiler.GetTime(KC_ProfileTimerType::RenderSystemGetEntitySet).asMicroseconds());
            ImGui::Text("Get Components: %dus * 10000", profiler.GetTime(KC_ProfileTimerType::RenderSystemGetComponent).asMicroseconds());
            ImGui::Text("Create: %dus * 10000", profiler.GetTime(KC_ProfileTimerType::RenderSystemCreateDrawnable).asMicroseconds());
            ImGui::Text("Draw: %dus * 10000", profiler.GetTime(KC_ProfileTimerType::RenderSystemDraw).asMicroseconds());
            ImGui::TreePop();
        }
    }
    ImGui::End();
}
} // Editor
} // ImGui
