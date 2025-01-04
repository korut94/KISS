#pragma once

#include "KC_ProfileTimerTypes.h"

#include <SFML/System/Clock.hpp>

class KC_ScopedProfileTimer final
{
public:
    KC_ScopedProfileTimer(KC_ProfileTimerType profileTimerType);
    ~KC_ScopedProfileTimer();

private:
    KC_ProfileTimerType myProfilerTimerType;
    sf::Clock myClock;
};
