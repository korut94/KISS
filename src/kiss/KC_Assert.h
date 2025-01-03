#pragma once

#if IS_DEBUG_BUILD
#include <cassert>
#define KC_ASSERT(anExpression, ...) assert((void(__VA_ARGS__), anExpression))
#define KC_STATIC_ASSERT(anExpression, aMessage) static_assert(anExpression, aMessage)
#else
#define KC_ASSERT(anExpression, ...)
#define KC_STATIC_ASSERT(anExpression, aMessage)
#endif // IS_DEBUG_BUILD
