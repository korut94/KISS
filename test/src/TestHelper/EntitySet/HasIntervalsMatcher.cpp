#include "HasIntervalsMatcher.h"

namespace TestHelper
{
namespace EntitySet
{
HasIntervalsMatcher::HasIntervalsMatcher(const std::vector<std::pair<KC_Entity, KC_EntitySet::EntityIndex>>& someExpectedIntervals)
    : myExpectedIntervals(someExpectedIntervals)
{
}

bool HasIntervalsMatcher::MatchAndExplain(const KC_EntitySet& anEntitySet, testing::MatchResultListener* aListener) const
{
    std::vector<std::pair<KC_Entity, KC_EntitySet::EntityIndex>> intervals;
    anEntitySet.GetAllIntervals(intervals);

    if (intervals == myExpectedIntervals)
    {
        return true;
    }
    else
    {
        *aListener << testing::PrintToString(intervals);
        return false;
    }
}

void HasIntervalsMatcher::DescribeTo(std::ostream* aListener) const
{
    *aListener << testing::PrintToString(myExpectedIntervals);
}

testing::Matcher<const KC_EntitySet&> HasIntervals(const std::vector<std::pair<KC_Entity, KC_EntitySet::EntityIndex>>& someExpectedIntervals)
{
    return testing::Matcher<const KC_EntitySet&>(new HasIntervalsMatcher(someExpectedIntervals));
}
} // namespace EntitySet
} // namespace TestHelper
