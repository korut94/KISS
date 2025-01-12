#include "TestHelper.h"

#include "KC_CircleRenderer.h"
#include "KC_ComponentManager.h"
#include "KC_EntitySet.h"
#include "KC_RectCollider.h"
#include "KC_Transform.h"

using namespace TestHelper;

class Test_KC_ComponentManager : public testing::Test
{
    using Components = std::tuple<KC_Transform, KC_CircleRenderer, KC_RectCollider>;

protected:
    void SetUp() override;

    KC_ComponentManager<Components> myComponentManager;
};

void Test_KC_ComponentManager::SetUp()
{
    // TODO: Make a test to assure the component manager is initialized with all value to nullptr
    myComponentManager.AddComponent<KC_Transform>(0);
    myComponentManager.AddComponent<KC_CircleRenderer>(0);

    myComponentManager.AddComponent<KC_Transform>(1);
    myComponentManager.AddComponent<KC_RectCollider>(1);

    myComponentManager.AddComponent<KC_Transform>(2);
    myComponentManager.AddComponent<KC_CircleRenderer>(2);

    myComponentManager.AddComponent<KC_Transform>(3);
}

TEST_F(Test_KC_ComponentManager, AddComponent)
{
    KC_Transform& transform = myComponentManager.GetComponent<KC_Transform>(0);
    EXPECT_EQ(transform.myPosition, sf::Vector2f(0.f, 0.f)) << "Must initialize the data inside properly";

    const sf::Vector2f translation = sf::Vector2f(10.f, 10.f); 
    transform.myPosition = translation;

    const KC_Transform& sameTransform = myComponentManager.GetComponent<KC_Transform>(0);
    EXPECT_EQ(transform.myPosition, translation) << "Must hold all the changes done on the component";
}

TEST_F(Test_KC_ComponentManager, AssignComponents)
{
    using SubComponents = std::tuple<KC_CircleRenderer, KC_RectCollider>;
    
    KC_ComponentManager<SubComponents> otherComponentManager;
    myComponentManager.AssignComponents(otherComponentManager);

    KC_EntitySet entitySet;
    
    otherComponentManager.GetEntitySet<KC_CircleRenderer>(entitySet);
    EXPECT_THAT(entitySet, EntitySet::Counts(2));
    EXPECT_THAT(entitySet, EntitySet::HasEntities({0, 2}));

    otherComponentManager.GetEntitySet<KC_RectCollider>(entitySet);
    EXPECT_THAT(entitySet, EntitySet::Counts(1));
    EXPECT_THAT(entitySet, EntitySet::HasEntities({1}));
}

TEST_F(Test_KC_ComponentManager, GetEntitySetMultipleComponentsDifferentOrder)
{
    KC_EntitySet entitySet;
    myComponentManager.GetEntitySet<KC_Transform, KC_CircleRenderer>(entitySet);

    KC_EntitySet otherEntitySet;
    myComponentManager.GetEntitySet<KC_CircleRenderer, KC_Transform>(otherEntitySet);

    EXPECT_EQ(entitySet, otherEntitySet) << "Must returns the same entity set regardless the arguments order";
}

TEST_F(Test_KC_ComponentManager, GetEntitySetMultipleComponentsSingleEntity)
{
    KC_EntitySet entitySet;
    myComponentManager.GetEntitySet<KC_Transform, KC_RectCollider>(entitySet);

    EXPECT_THAT(entitySet, EntitySet::Counts(1)) << "Must select only the entity '1' having both KC_Transform and KC_RectangleRenderer";
    EXPECT_THAT(entitySet, EntitySet::HasEntities({1}));
    EXPECT_THAT(entitySet, EntitySet::HasIntervals({{1, 1}}));
}

TEST_F(Test_KC_ComponentManager, GetEntitySetSingleComponentAllEntities)
{
    KC_EntitySet entitySet;
    myComponentManager.GetEntitySet<KC_Transform>(entitySet);

    EXPECT_THAT(entitySet, EntitySet::Counts(4));
    EXPECT_THAT(entitySet, EntitySet::HasEntities({0, 1, 2, 3}));
    EXPECT_THAT(entitySet, EntitySet::HasIntervals({{0, 4}}));
}

TEST_F(Test_KC_ComponentManager, GetEntitySetSingleComponentSomeEntities)
{
    KC_EntitySet entitySet;
    myComponentManager.GetEntitySet<KC_CircleRenderer>(entitySet);
    
    EXPECT_THAT(entitySet, EntitySet::Counts(2));
    EXPECT_THAT(entitySet, EntitySet::HasEntities({0, 2}));
    EXPECT_THAT(entitySet, EntitySet::HasIntervals({{0, 1}, {2, 1}}));
}
