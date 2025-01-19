#pragma once

#include <chrono>

class KC_Time final
{
    using Clock = std::chrono::steady_clock;

public:
    KC_Time() = default;
    template <typename TRep, typename TPeriod>
    KC_Time(const std::chrono::duration<TRep, TPeriod>& aDuration);

    float AsSeconds() const;
    std::int32_t AsMilliseconds() const;
    std::int64_t AsMicroseconds() const;
    std::int64_t AsNanoseconds() const;

    static KC_Time Now();
    static KC_Time Seconds(float anAmount);
    static KC_Time Milliseconds(std::int32_t anAmount);
    static KC_Time Microseconds(std::int64_t anAmount);
    static KC_Time Nanoseconds(std::int64_t anAmount);

    static void SetFrameZero();

private:
    std::chrono::nanoseconds myNanoseconds;

    static Clock::time_point ourFrameZero;
};

bool operator==(KC_Time a, KC_Time b);
bool operator!=(KC_Time a, KC_Time b);
bool operator<(KC_Time a, KC_Time b);
bool operator>(KC_Time a, KC_Time b);
bool operator<=(KC_Time a, KC_Time b);
bool operator>=(KC_Time a, KC_Time b);
KC_Time operator-(KC_Time a, KC_Time b);
KC_Time& operator-=(KC_Time& a, KC_Time b);

template <typename TRep, typename TPeriod>
KC_Time::KC_Time(const std::chrono::duration<TRep, TPeriod>& aDuration)
    : myNanoseconds(aDuration)
{
}
