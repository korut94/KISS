#include "KC_Math.h"

#include <cmath>

namespace KC_Math
{
    float DegToRad(float aDegree)
    {
        return aDegree * M_PI / 180.0f;
    }

    float RadToDeg(float aRadian)
    {
        return aRadian * 180.0f / M_PI;
    }

    std::int32_t Sign(std::int32_t aValue)
    {
        return (aValue > 0) - (aValue < 0);
}
} // namespace KC_Math
