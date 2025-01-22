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
    int32_t AsMilliseconds() const;
    int64_t AsMicroseconds() const;
    int64_t AsNanoseconds() const;

    static KC_Time Now();
    static KC_Time Seconds(float anAmount);
    static KC_Time Milliseconds(int32_t anAmount);
    static KC_Time Microseconds(int64_t anAmount);
    static KC_Time Nanoseconds(int64_t anAmount);

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
KC_Time operator+(KC_Time a, KC_Time b);
KC_Time& operator-=(KC_Time& a, KC_Time b);
KC_Time& operator+=(KC_Time& a, KC_Time b);

template <typename TRep, typename TPeriod>
KC_Time::KC_Time(const std::chrono::duration<TRep, TPeriod>& aDuration)
    : myNanoseconds(aDuration)
{
}
