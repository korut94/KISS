#pragma once

#include "KC_Game.h"

class MC_Game final : public KC_Game
{
    using Super = KC_Game;

public:
    explicit MC_Game(KC_World& aWorld);

    void Init();
    void Update();

private:
    KC_World& myWorld;
};
