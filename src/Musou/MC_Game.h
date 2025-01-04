#pragma once

#include "KC_Game.h"

class MC_Game final : public KC_Game
{
public:
    MC_Game();

    void Update(KC_World& aWorld);

private:
    bool myIsInitialized;
};
