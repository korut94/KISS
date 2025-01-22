#pragma once

#include "KC_Entity.h"

#include <vector>

class KC_EntitySet final
{
    friend class Iterator;

public:
    using EntityIndex = KC_Entity;

    class Iterator
    {
    public:
        Iterator(const KC_EntitySet& anEntitySet, size_t anIndex);

        bool operator==(const Iterator& anOther) const;
        bool operator!=(const Iterator& anOther) const;
        KC_Entity operator*() const;
        Iterator& operator++();
        Iterator operator++(int);

    private:
        const KC_EntitySet& myEntitySet;
        size_t myIndex;
        EntityIndex myCount;
    };

private:
    struct Interval
    {
        KC_Entity myFirstEntity = 0;
        EntityIndex myTotalCount = 0;
    };

public:
    KC_EntitySet() = default;
    KC_EntitySet(KC_Entity anEntity);

    Iterator begin() const;
    Iterator Begin() const;
    Iterator end() const;
    Iterator End() const;

    KC_Entity At(EntityIndex anIndex) const;
    EntityIndex Count() const;
    bool Contains(KC_Entity anEntity) const;
    EntityIndex GetIndex(KC_Entity anEntity) const;
    KC_EntitySet Intersect(const KC_EntitySet& anOther) const;
    bool IsEmpty() const;

    void Insert(KC_Entity anEntity);

    bool operator==(const KC_EntitySet& anOther) const;
    KC_EntitySet& operator=(const KC_EntitySet& anOther);

#if IS_DEBUG_BUILD
    void GetAllEntities(std::vector<KC_Entity>& outSomeEntities) const;
    void GetAllIntervals(std::vector<std::pair<KC_Entity, EntityIndex>>& outSomeIntervals) const;
    size_t GetIntervalsCount() const { return myIntervals.size(); }
#endif // IS_DEBUG_BUILD

private:
    std::vector<Interval> myIntervals;
};
