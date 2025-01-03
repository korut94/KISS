#pragma once

#include "KC_Game.h"

template <typename TGame>
class KT_Editor final : public KC_Game
{
public:
    void ImGui();
    void Update();

private:
    TGame myGame;
};

template <typename TGame>
void KT_Editor<TGame>::ImGui()
{
    myGame.ImGui();
}

template <typename TGame>
void KT_Editor<TGame>::Update()
{
    myGame.Update();
}
