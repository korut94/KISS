#include "HasEntitiesMatcher.h"

#include "KC_EntitySet.h"

#include <gmock/gmock.h>

namespace TestHelper
{
namespace EntitySet
{
TestHelper::EntitySet::HasEntitiesMatcher::HasEntitiesMatcher(const std::vector<KC_Entity>& someExpectedEntities)
    : myExpectedEntities(someExpectedEntities)
{
}

bool TestHelper::EntitySet::HasEntitiesMatcher::MatchAndExplain(const KC_EntitySet& anEntitySet, testing::MatchResultListener* aListener) const
{
    std::vector<KC_Entity> entities;
    anEntitySet.GetAllEntities(entities);

    if (entities == myExpectedEntities)
    {
        return true;
    }
    else
    {
        *aListener << testing::PrintToString(entities);
        return false;
    }
}

void TestHelper::EntitySet::HasEntitiesMatcher::DescribeTo(std::ostream* aListener) const
{ 
    *aListener << testing::PrintToString(myExpectedEntities);
}

testing::Matcher<const KC_EntitySet&> HasEntities(const std::vector<KC_Entity>& someExpectedEntities)
{
    return testing::Matcher<const KC_EntitySet&>(new HasEntitiesMatcher(someExpectedEntities));
}
} // namespace EntitySet
} // namespace TestHelper
