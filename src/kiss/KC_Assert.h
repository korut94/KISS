#pragma once

#include <cassert>

#define KC_ASSERT(anExpression, ...) assert((void(__VA_ARGS__), anExpression))
#define KC_STATIC_ASSERT(anExpression, aMessage) static_assert(anExpression, aMessage)
