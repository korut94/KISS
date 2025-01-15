#include "KC_Math.h"

namespace KC_Math
{
std::int32_t Sign(std::int32_t aValue)
{
    return (aValue > 0) - (aValue < 0);
}
} // namespace KC_Math
