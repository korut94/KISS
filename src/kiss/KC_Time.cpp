#include "KC_Time.h"

#include "KC_Assert.h"

KC_Time::Clock::time_point KC_Time::ourFrameZero;

float KC_Time::AsSeconds() const
{
    return std::chrono::duration<float>(myNanoseconds).count();
}

int32_t KC_Time::AsMilliseconds() const
{
    return std::chrono::duration_cast<std::chrono::duration<int32_t, std::milli>>(myNanoseconds).count();
}

int64_t KC_Time::AsMicroseconds() const
{
    return std::chrono::duration_cast<std::chrono::microseconds>(myNanoseconds).count();
}

int64_t KC_Time::AsNanoseconds() const
{
    return myNanoseconds.count();
}

KC_Time KC_Time::Now()
{
    return Clock::now() - ourFrameZero;
}

KC_Time KC_Time::Seconds(float anAmount)
{
    return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::duration<float>(anAmount));
}

KC_Time KC_Time::Milliseconds(int32_t anAmount)
{
    return std::chrono::milliseconds(anAmount);
}

KC_Time KC_Time::Microseconds(int64_t anAmount)
{
    return std::chrono::microseconds(anAmount);
}

KC_Time KC_Time::Nanoseconds(int64_t anAmount)
{
    return std::chrono::nanoseconds(anAmount);
}

void KC_Time::SetFrameZero()
{
    KC_ASSERT(ourFrameZero.time_since_epoch() == Clock::duration::zero(), "Did you call SetFrameZero more than once?");
    ourFrameZero = Clock::now();
}

bool operator==(KC_Time a, KC_Time b)
{
    return a.AsNanoseconds() == b.AsNanoseconds();
}

bool operator!=(KC_Time a, KC_Time b)
{
    return a.AsNanoseconds() != b.AsNanoseconds();
}

bool operator<(KC_Time a, KC_Time b)
{
    return a.AsNanoseconds() < b.AsNanoseconds();
}

bool operator>(KC_Time a, KC_Time b)
{
    return a.AsNanoseconds() > b.AsNanoseconds();
}

bool operator<=(KC_Time a, KC_Time b)
{
    return a.AsNanoseconds() <= b.AsNanoseconds();
}

bool operator>=(KC_Time a, KC_Time b)
{
    return a.AsNanoseconds() >= b.AsNanoseconds();
}

KC_Time operator-(KC_Time a, KC_Time b)
{
    return KC_Time::Nanoseconds(a.AsNanoseconds() - b.AsNanoseconds());
}

KC_Time operator+(KC_Time a, KC_Time b)
{
    return KC_Time::Nanoseconds(a.AsNanoseconds() + b.AsNanoseconds());
}

KC_Time& operator-=(KC_Time& a, KC_Time b)
{
    a = a - b;
    return a;
}

KC_Time& operator+=(KC_Time& a, KC_Time b)
{
    a = a + b;
    return a;
}
