#pragma once

#include "KC_Entity.h"

#include <gtest/gtest.h>

#include <vector>

class KC_EntitySet;

namespace TestHelper
{
namespace EntitySet
{
class HasEntitiesMatcher : public testing::MatcherInterface<const KC_EntitySet&>
{
public:
    explicit HasEntitiesMatcher(const std::vector<KC_Entity>& someExpectedEntities);
    bool MatchAndExplain(const KC_EntitySet& anEntitySet, testing::MatchResultListener* aListener) const override;
    void DescribeTo(std::ostream* aListener) const;

private:
    const std::vector<KC_Entity> myExpectedEntities;
};

testing::Matcher<const KC_EntitySet&> HasEntities(const std::vector<KC_Entity>& someExpectedEntities);
} // namespace EntitySet
} // namespace TestHelper
