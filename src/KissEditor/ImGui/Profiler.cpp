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
    ImGui::Text("Game: %dms", profiler.GetTime(KC_ProfileTimerType::Game).asMilliseconds());
    ImGui::Text("Rendering: %dms", profiler.GetTime(KC_ProfileTimerType::Rendering).asMilliseconds());
    ImGui::Text("Rendering Wait: %dms", profiler.GetTime(KC_ProfileTimerType::RenderingWait).asMilliseconds());
    ImGui::Text("GameRender Sync: %dms", profiler.GetTime(KC_ProfileTimerType::GameRenderSync).asMilliseconds());
    ImGui::End();
}
} // Editor
} // ImGui
