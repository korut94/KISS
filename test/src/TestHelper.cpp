#include "TestHelper.h"

#include "KC_EntitySet.h"

void PrintTo(const KC_EntitySet& anEntitySet, std::ostream* os)
{
    std::vector<KC_Entity> entities;
    anEntitySet.GetAllEntities(entities);

    *os << testing::PrintToString(entities);
}
