#pragma once

#include "KC_ProfileTimerTypes.h"

#include <SFML/System/Time.hpp>

#include <array>

class KC_Profiler final
{
public:
    KC_Profiler();

    sf::Time GetTime(KC_ProfileTimerType aProfileTimerType) const;
    void SetTime(KC_ProfileTimerType aProfileTimerType, sf::Time aTime);

    static KC_Profiler& GetProfiler();

private:
    std::array<sf::Time, KC_ProfileTimerType::Count> myTimes;

    static KC_Profiler* myInstance;
};
