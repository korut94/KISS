#pragma once

#include <cstdint>
#include <limits>

namespace KC_Math
{
bool AreValuesClose(float a, float b, float delta = std::numeric_limits<float>::min());
float DegToRad(float aDegree);
float RadToDeg(float aRadian);
int32_t Sign(int32_t aValue);
} // namespace KC_Math
