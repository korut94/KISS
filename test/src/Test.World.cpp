// Use Case - World https://miro.com/app/board/uXjVL3mClFw=/?moveToWidget=3458764610495215024&cot=14

#include "KC_EntityManager.h"
#include "KC_World.h"

#include <gtest/gtest.h>

class Test_World : public testing::Test
{
protected:
    KC_World myWorld;
};

TEST_F(Test_World, CreateEntity)
{
    EXPECT_EQ(myWorld.GetEntityManager().CreateEntity(), 0);
    // Guarantee incremental id
    EXPECT_EQ(myWorld.GetEntityManager().CreateEntity(), 1) << "H_World::CreatyEntity must return an incremental value";
}

TEST_F(Test_World, DestroyEntity)
{
    SUCCEED();
}
