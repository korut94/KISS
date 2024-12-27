#pragma once

#include "KC_EntitySet.h"

#include <gtest/gtest.h>

#include <vector>

namespace TestHelper
{
namespace EntitySet
{
class HasIntervalsMatcher : public testing::MatcherInterface<const KC_EntitySet&>
{
public:
    explicit HasIntervalsMatcher(const std::vector<std::pair<KC_Entity, KC_EntitySet::EntityIndex>>& someExpectedIntervals);
    bool MatchAndExplain(const KC_EntitySet& anEntitySet, testing::MatchResultListener* aListener) const override;
    void DescribeTo(std::ostream* aListener) const;

private:
    const std::vector<std::pair<KC_Entity, KC_EntitySet::EntityIndex>> myExpectedIntervals;
};

testing::Matcher<const KC_EntitySet&> HasIntervals(const std::vector<std::pair<KC_Entity, KC_EntitySet::EntityIndex>>& someExpectedIntervals);
} // namespace EntitySet
} // namespace TestHelper
