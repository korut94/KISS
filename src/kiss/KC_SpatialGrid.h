#pragma once

#include "KC_EntitySet.h"
#include "KC_Rect.h"

#include <SFML/System/Vector2.hpp>

#include <unordered_map>

class KC_SpatialGrid final
{
public:
    KC_SpatialGrid(std::int32_t aGridCellSize);

    std::int32_t GetGridCellSize() const { return myGridCellSize; }
    sf::Vector2i GetGridCoordinate(std::int32_t anIndex) const;
    void GetGridCoordinates(std::vector<sf::Vector2i>& outSomeGridCoordinates) const;
    void GetIndexs(std::vector<std::int32_t>& outSomeIndexes) const;

    void Clear();
    void InsertEntity(KC_Entity anEntity, const KC_FloatRect& aBoundingRect);

#if IS_DEBUG_BUILD
    const std::unordered_map<std::int32_t, KC_EntitySet>& GetGridCells() const { return myGridCells; };
#endif // IS_DEBUG_BUILD

private:
    std::int32_t GetIndex(const sf::Vector2f& aPosition) const;
    constexpr std::int32_t GetOneDimensionCellsCount() const;

    const std::int32_t myGridCellSize;
    std::unordered_map<std::int32_t, KC_EntitySet> myGridCells;
};
