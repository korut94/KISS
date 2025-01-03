#include "MC_Game.h"

#if IS_IMGUI
#include "imgui.h"
#endif // IS_IMGUI

void MC_Game::Update()
{
}

#if IS_IMGUI
void MC_Game::ImGui()
{
    ImGui::ShowDemoWindow();
}
#endif // IS_IMGUI
