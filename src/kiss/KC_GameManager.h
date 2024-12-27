#pragma once

#include "KC_World.h"

namespace sf { class RenderWindow; }

class KC_GameManager final
{
public:
    KC_GameManager();

    const KC_World& GetWorld() const;
    KC_World& GetWorld();

    void Run();

private:
    bool ProcessEvent(sf::RenderWindow& aRenderWindow);

    KC_World* myWorld;
};
