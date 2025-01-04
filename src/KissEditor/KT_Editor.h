#pragma once

#include "KC_Game.h"

class KC_World;

template <typename TGame>
class KT_Editor final : public KC_Game
{
public:
    void Update(KC_World& aWorld);
#if IS_IMGUI
    void ImGui();
#endif // IS_IMGUI

private:
    TGame myGame;
};

template <typename TGame>
void KT_Editor<TGame>::Update(KC_World& aWorld)
{
    myGame.Update(aWorld);
}

#if IS_IMGUI
template <typename TGame>
void KT_Editor<TGame>::ImGui()
{
    myGame.ImGui();
}
#endif // IS_IMGUI
