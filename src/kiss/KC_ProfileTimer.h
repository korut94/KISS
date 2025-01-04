#pragma once

#include <SFML/System/Clock.hpp>

class KC_ProfileTimer final
{
public:
    explicit KC_ProfileTimer(const char* aTimerName);

    void ResetTime();
    void RecordTime();

private:
    const char* const myTimerName;
    sf::Clock myClock;
};
