#pragma once

#include "KC_ProfileTimerTypes.h"

#include <SFML/System/Clock.hpp>

class KC_ProfileTimer final
{
public:
    explicit KC_ProfileTimer(KC_ProfileTimerType aProfilerTimerType);

    void ResetTime();
    void RecordTime();

private:
    KC_ProfileTimerType myProfilerTimerType;
    sf::Clock myClock;
};
