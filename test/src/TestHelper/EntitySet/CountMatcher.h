#pragma once

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace TestHelper
{
namespace EntitySet
{
MATCHER_P(Counts, aCount, testing::PrintToString(aCount))
{ 
    *result_listener << arg.Count(); 
    return arg.Count() == aCount;
}
} // namespace EntitySet
} // namespace TestHelper
