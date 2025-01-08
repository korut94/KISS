#include "KC_EntitySet.h"

#include "KC_Assert.h"

KC_EntitySet::Iterator::Iterator(const KC_EntitySet& anEntitySet, std::size_t anIndex)
    : myEntitySet(anEntitySet)
    , myIndex(anIndex)
    , myCount(0)
{
    KC_ASSERT(anIndex <= myEntitySet.myIntervals.size(), "Declared iterator out of bound");
}

bool KC_EntitySet::Iterator::operator==(const Iterator& anOther) const
{
    return &myEntitySet == &anOther.myEntitySet && myIndex == anOther.myIndex && myCount == anOther.myCount;
}

bool KC_EntitySet::Iterator::operator!=(const Iterator& anOther) const
{
    return !(*this == anOther);
}

KC_Entity KC_EntitySet::Iterator::operator*() const
{
    KC_ASSERT(myIndex < myEntitySet.myIntervals.size(), "Try to dereferencing the iterator end");
    return myEntitySet.myIntervals[myIndex].myFirstEntity + myCount;
}

KC_EntitySet::Iterator& KC_EntitySet::Iterator::operator++()
{
    if (myIndex < myEntitySet.myIntervals.size())
    {
        if (myCount + 1 < myEntitySet.myIntervals[myIndex].myTotalCount)
        {
            myCount++;
        }
        else
        {
            myIndex++;
            myCount = 0;
        }
    }

    return *this;
}

KC_EntitySet::Iterator KC_EntitySet::Iterator::operator++(int)
{
    Iterator itr = *this;
    ++(*this);

    return itr;
}

KC_EntitySet::Iterator KC_EntitySet::begin() const
{
    return Iterator(*this, 0);
}

KC_EntitySet::Iterator KC_EntitySet::Begin() const
{
    return Iterator(*this, 0);
}

KC_EntitySet::Iterator KC_EntitySet::end() const
{
    return Iterator(*this, myIntervals.size());
}

KC_EntitySet::Iterator KC_EntitySet::End() const
{
    return Iterator(*this, myIntervals.size());
}

KC_Entity KC_EntitySet::At(EntityIndex anIndex) const
{
    std::size_t intervalIndex = 0;
    const std::size_t intervalCount = myIntervals.size();

    while (intervalIndex < intervalCount && myIntervals[intervalIndex].myTotalCount < anIndex)
    {
        anIndex -= myIntervals[intervalIndex].myTotalCount;
        ++intervalIndex;
    }

    KC_ASSERT(intervalIndex < intervalCount, "Entity index out of bound");
    return myIntervals[intervalIndex].myFirstEntity + anIndex;
}

KC_EntitySet::EntityIndex KC_EntitySet::Count() const
{
    EntityIndex count = 0;
    for (const Interval& interval : myIntervals)
    {
        count += interval.myTotalCount;
    }

    return count;
}

bool KC_EntitySet::Contains(KC_Entity anEntity) const
{
    for (const Interval& interval : myIntervals)
    {
        if (interval.myFirstEntity <= anEntity && anEntity < interval.myFirstEntity + interval.myTotalCount)
            return true;
    }

    return false;
}

KC_EntitySet::EntityIndex KC_EntitySet::GetIndex(KC_Entity anEntity) const
{
    KC_ASSERT(myIntervals.size() > 0, "EntitySet is empty");
    KC_ASSERT(anEntity < myIntervals.back().myFirstEntity + myIntervals.back().myTotalCount, "Entity doesn't belong to this set");

    auto itr = myIntervals.cbegin();
    EntityIndex count = 0;

    while (anEntity > itr->myFirstEntity + itr->myTotalCount)
    {
        count += itr->myTotalCount;
        ++itr;
    }

    KC_ASSERT(itr->myFirstEntity <= anEntity && anEntity < itr->myFirstEntity + itr->myTotalCount, "Entity doesn't belong to this set");
    return (anEntity - itr->myFirstEntity) + count;
}

KC_EntitySet KC_EntitySet::Intersect(const KC_EntitySet& anOther) const
{
    KC_EntitySet result;

    const bool hasLessIntervals = myIntervals.size() <= anOther.myIntervals.size();

    const std::vector<Interval>& sourceIntervals = hasLessIntervals ? myIntervals : anOther.myIntervals;
    const std::vector<Interval>& targetIntervals = hasLessIntervals ? anOther.myIntervals : myIntervals;

    const std::size_t targetSize = targetIntervals.size();
    std::size_t targetIndex = 0;

    bool isConsumed = false;

    for (const Interval& source : sourceIntervals)
    {
        isConsumed = false;

        while (!isConsumed && targetIndex < targetSize)
        {
            const Interval& target = targetIntervals[targetIndex];

            const EntityIndex sourceLeft = source.myFirstEntity;
            const EntityIndex sourceRight = source.myFirstEntity + source.myTotalCount - 1;
            const EntityIndex targetLeft = target.myFirstEntity;
            const EntityIndex targetRight = target.myFirstEntity + target.myTotalCount - 1;

            if (sourceLeft < targetLeft && targetLeft <= sourceRight && sourceRight < targetRight)
            {
                result.myIntervals.push_back({ targetLeft, sourceRight - targetLeft });
                isConsumed = true;
            }
            else if (targetLeft <= sourceLeft && sourceRight <= targetRight)
            {
                result.myIntervals.push_back(source);
                isConsumed = true;
            }
            else if (targetLeft < sourceLeft && sourceLeft <= targetRight && targetRight < sourceRight)
            {
                result.myIntervals.push_back({ sourceLeft, targetRight - sourceLeft });
                targetIndex++;
            }
            else if (sourceLeft <= targetLeft && targetRight <= sourceRight)
            {
                result.myIntervals.push_back(target);
                targetIndex++;
            }
            else if (targetRight < sourceLeft)
            {
                targetIndex++;
            }
            else if (sourceRight < targetLeft)
            {
                isConsumed = true;
            }
        }
    }

    return result;
}

bool KC_EntitySet::IsEmpty() const
{
    return myIntervals.size() == 0;
}

void KC_EntitySet::Insert(KC_Entity anEntity)
{
    if (myIntervals.size() == 0)
    {
        myIntervals.push_back({ anEntity, 1 });
        return;
    }

    const std::size_t count = myIntervals.size();

    auto itr = myIntervals.begin();
    const auto end = myIntervals.end();
    while (itr < end && anEntity > itr->myFirstEntity + itr->myTotalCount)
    {
        itr++;
    }

    if (itr == end)
    {
        myIntervals.push_back({ anEntity, 1 });
        return;
    }

    Interval& interval = *itr;
    if (anEntity < interval.myFirstEntity)
    {
        // The first entity must be greater than 0 if we get inside this branch 
        if (anEntity + 1 == interval.myFirstEntity)
        {
            interval.myFirstEntity = anEntity;
            interval.myTotalCount++;
        }
        else
        {
            // Very bad
            myIntervals.insert(itr, { anEntity, 1 });
        }
    }
    else
    {
        KC_ASSERT(anEntity == interval.myFirstEntity + interval.myTotalCount, "Trying to add to the set the same element");
        
        if (itr + 1 == end || anEntity + 1 < (itr + 1)->myFirstEntity)
        {
            interval.myTotalCount++;
        }
        else
        {
            interval.myTotalCount = interval.myTotalCount + (itr + 1)->myTotalCount + 1;
            // Very bad
            myIntervals.erase(itr + 1);
        }
    }
}

bool KC_EntitySet::operator==(const KC_EntitySet& anOther) const
{
    if (myIntervals.size() != anOther.myIntervals.size())
        return false;

    for (std::size_t i = 0, count = myIntervals.size(); i < count; ++i)
    {
        if (myIntervals[i].myFirstEntity != anOther.myIntervals[i].myFirstEntity || myIntervals[i].myTotalCount != anOther.myIntervals[i].myTotalCount)
            return false;
    }

    return true;
}

KC_EntitySet& KC_EntitySet::operator=(const KC_EntitySet& anOther)
{
    myIntervals = anOther.myIntervals;
    return *this;
}

#if IS_DEBUG_BUILD
void KC_EntitySet::GetAllEntities(std::vector<KC_Entity>& outSomeEntities) const
{
    for (const Interval& interval : myIntervals)
    {
        for (KC_Entity i = 0, count = interval.myTotalCount; i < count; ++i)
        {
            outSomeEntities.push_back(interval.myFirstEntity + i);
        }
    }
}

void KC_EntitySet::GetAllIntervals(std::vector<std::pair<KC_Entity, EntityIndex>>& outSomeIntervals) const
{
    for (const Interval& interval : myIntervals)
    {
        outSomeIntervals.push_back(std::make_pair(interval.myFirstEntity, interval.myTotalCount));
    }
}
#endif // IS_DEBUG_BUILD
