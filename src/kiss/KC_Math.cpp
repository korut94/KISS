#include "KC_Math.h"

#include <cmath>

namespace KC_Math
{
    bool AreValuesClose(float a, float b, float delta /*= std::numeric_limits<float>::min()*/)
    {
        return std::abs(a - b) < delta;
    }

    float DegToRad(float aDegree)
    {
        return aDegree * M_PI / 180.0f;
    }

    float RadToDeg(float aRadian)
    {
        return aRadian * 180.0f / M_PI;
    }

    int32_t Sign(int32_t aValue)
    {
        return (aValue > 0) - (aValue < 0);
}
} // namespace KC_Math
