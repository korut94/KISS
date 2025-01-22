#include "TestHelper.h"

#include "KC_EntitySet.h"

using namespace TestHelper::EntitySet;

class Test_KC_EntitySet : public testing::Test
{
protected:
    void SetUp() override;

    KC_EntitySet myEntitySetA;
    KC_EntitySet myEntitySetB;
    KC_EntitySet myEntitySetC;
};

void Test_KC_EntitySet::SetUp()
{
    myEntitySetA.Insert(2);
    myEntitySetA.Insert(3);
    myEntitySetA.Insert(4);
    myEntitySetA.Insert(6);
    myEntitySetA.Insert(7);
    myEntitySetA.Insert(11);

    myEntitySetB.Insert(0);
    myEntitySetB.Insert(2);
    myEntitySetB.Insert(3);
    myEntitySetB.Insert(9);
    myEntitySetB.Insert(10);
    myEntitySetB.Insert(11);
    myEntitySetB.Insert(12);
    myEntitySetB.Insert(20);

    myEntitySetC.Insert(15);
    myEntitySetC.Insert(16);
}

TEST_F(Test_KC_EntitySet, GetIndex)
{
    // https://miro.com/app/board/uXjVL3mClFw=/?moveToWidget=3458764610783752038&cot=14

    std::vector<KC_Entity> entities;
    myEntitySetA.GetAllEntities(entities);

    for (size_t i = 0, count = entities.size(); i < count; ++i)
    {
        EXPECT_EQ(myEntitySetA.GetIndex(entities[i]), i);
    }
}

TEST_F(Test_KC_EntitySet, GetIndexInvalid)
{
    EXPECT_DEATH
    ({
        myEntitySetA.GetIndex(5);
    }, "");
}

TEST_F(Test_KC_EntitySet, Insert)
{
    EXPECT_THAT(myEntitySetA, Counts(6));
    EXPECT_THAT(myEntitySetA, HasEntities({ 2, 3, 4, 6, 7, 11 }));
    EXPECT_THAT(myEntitySetA, HasIntervals({{ 2, 3 }, { 6, 2 }, { 11, 1 }}));

    EXPECT_THAT(myEntitySetB, Counts(8));
    EXPECT_THAT(myEntitySetB, HasEntities({ 0, 2, 3, 9, 10, 11, 12, 20 }));
    EXPECT_THAT(myEntitySetB, HasIntervals({{ 0, 1 }, { 2, 2 }, { 9, 4 }, { 20, 1 }}));
}

TEST_F(Test_KC_EntitySet, InsertBetweenMiddles)
{
    myEntitySetA.Insert(5); // The middles are 4 and 6

    EXPECT_THAT(myEntitySetA, Counts(7));
    EXPECT_THAT(myEntitySetA, HasEntities({ 2, 3, 4, 5, 6, 7, 11 }));
    EXPECT_THAT(myEntitySetA, HasIntervals({{ 2, 6 }, { 11, 1 }})) << "The two middle intervals, (2, 3) and (6, 2), must be merged togheter";
}

TEST_F(Test_KC_EntitySet, InsertEqualFirstMinusOne)
{
    myEntitySetA.Insert(1);

    EXPECT_THAT(myEntitySetA, Counts(7)) << "The set cardinality must increase by one";
    EXPECT_THAT(myEntitySetA, HasEntities({ 1, 2, 3, 4, 6, 7, 11 }));
    EXPECT_THAT(myEntitySetA, HasIntervals({{ 1, 4 }, { 6, 2 }, { 11, 1 }})) << "Only the first entity of the first entity must change with the new value 1";
}

TEST_F(Test_KC_EntitySet, InsertEqualLastPlusOne)
{
    myEntitySetA.Insert(12);
    EXPECT_THAT(myEntitySetA, Counts(7)) << "The set cardinality must increase by one";
    EXPECT_THAT(myEntitySetA, HasEntities({ 2, 3, 4, 6, 7, 11, 12 }));
    EXPECT_THAT(myEntitySetA, HasIntervals({{ 2, 3 }, { 6, 2 }, { 11, 2 }})) << "Only last interval count must increase by one";

    myEntitySetC.Insert(17);
    EXPECT_THAT(myEntitySetC, Counts(3)) << "The set cardinality must increase by one";
    EXPECT_THAT(myEntitySetC, HasEntities({ 15, 16, 17 }));
    EXPECT_THAT(myEntitySetC, HasIntervals({{ 15, 3 }})) << "There must be still only one interval";
}

TEST_F(Test_KC_EntitySet, InsertEqualMiddleMinusOne)
{
    myEntitySetA.Insert(10); // The middle is 11

    EXPECT_THAT(myEntitySetA, Counts(7)) << "The set cardinality must increase by one";
    EXPECT_THAT(myEntitySetA, HasEntities({ 2, 3, 4, 6, 7, 10, 11 }));
    EXPECT_THAT(myEntitySetA, HasIntervals({{ 2, 3 }, { 6, 2 }, { 10, 2 }})) << "Only middle interval first entity must change with the new value 10";
}

TEST_F(Test_KC_EntitySet, InsertEqualMiddlePlusOne)
{
    myEntitySetA.Insert(8); // The middle is 7

    EXPECT_THAT(myEntitySetA, Counts(7)) << "The set cardinality must increase by one";
    EXPECT_THAT(myEntitySetA, HasEntities({ 2, 3, 4, 6, 7, 8, 11 }));
    EXPECT_THAT(myEntitySetA, HasIntervals({{ 2, 3 }, { 6, 3 }, { 11, 1 }})) << "Only middle interval count must inscrease by one";
}

TEST_F(Test_KC_EntitySet, InsertExistingEntity)
{
    EXPECT_DEATH
    ({
        myEntitySetA.Insert(3);
    }, "");
}

TEST_F(Test_KC_EntitySet, InsertGreaterThanLastPlusOne)
{
    myEntitySetA.Insert(13);

    EXPECT_THAT(myEntitySetA, Counts(7)) << "The set cardinality must increase by one";
    EXPECT_THAT(myEntitySetA, HasEntities({ 2, 3, 4, 6, 7, 11, 13 }));
    EXPECT_THAT(myEntitySetA, HasIntervals({{ 2, 3 }, { 6, 2 }, { 11, 1 }, { 13, 1 }})) << "The new interval (13, 1) must be appended at the back of the list";
}

TEST_F(Test_KC_EntitySet, InsertGreaterThanMiddlePlusOne)
{
    myEntitySetA.Insert(9); // The middle is 7

    EXPECT_THAT(myEntitySetA, Counts(7)) << "The set cardinality must increase by one";
    EXPECT_THAT(myEntitySetA, HasEntities({ 2, 3, 4, 6, 7, 9, 11 }));
    EXPECT_THAT(myEntitySetA, HasIntervals({{ 2, 3 }, { 6, 2 }, { 9, 1 }, { 11, 1 }})) << "The new interval (9, 1) must be appended in the middle of the list";
}

TEST_F(Test_KC_EntitySet, InsertLessThanFirstMinusOne)
{
    myEntitySetA.Insert(0);
    
    EXPECT_THAT(myEntitySetA, Counts(7)) << "The set cardinality must increase by one";
    EXPECT_THAT(myEntitySetA, HasEntities({ 0, 2, 3, 4, 6, 7, 11 }));
    EXPECT_THAT(myEntitySetA, HasIntervals({{ 0, 1 }, { 2, 3 }, { 6, 2 }, { 11, 1 }})) << "The new interval (0, 1) must be appended in front at the list";
}

TEST_F(Test_KC_EntitySet, Intersect)
{
    // https://miro.com/app/board/uXjVL3mClFw=/?moveToWidget=3458764610783752038&cot=14
    
    KC_EntitySet intersection = myEntitySetB.Intersect(myEntitySetA);

    EXPECT_THAT(intersection, Counts(3));
    EXPECT_THAT(intersection, HasEntities({ 2, 3, 11 }));
    EXPECT_THAT(intersection, HasIntervals({{ 2, 2 }, { 11, 1 }}));
}

TEST_F(Test_KC_EntitySet, IntersectWithDisjointSet)
{
    KC_EntitySet intersect = myEntitySetA.Intersect(myEntitySetC);
    EXPECT_THAT(intersect, Counts(0));
}

TEST_F(Test_KC_EntitySet, IntersectWithEmptySet)
{
    KC_EntitySet intersect = myEntitySetA.Intersect(KC_EntitySet());
    EXPECT_THAT(intersect, Counts(0));
}
