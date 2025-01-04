#pragma once

#include <SFML/System/Time.hpp>

#include <string>
#include <unordered_map>

// No thread safe. It might be necesarray change it in the future but, for the moment,
// it doesn't harm if we don't read the correct timer value.
class KC_Profiler final
{
public:
    KC_Profiler();

    sf::Time GetTime(const char* aTimerName) const;
    void SetTime(const char* aTimerName, sf::Time aTime);

    static KC_Profiler& GetProfiler();

private:
    std::unordered_map<const char*, sf::Time> myTimes;

    static KC_Profiler* myInstance;
};
