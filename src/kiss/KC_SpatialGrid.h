#pragma once

#include "KC_EntitySet.h"
#include "KC_Rect.h"

#include <SFML/System/Vector2.hpp>

#include <unordered_map>

class KC_SpatialGrid final
{
public:
    KC_SpatialGrid(std::int32_t aGridCellSize);

    std::int32_t GetIndex(const sf::Vector2f& aPosition) const;

    void InsertEntity(KC_Entity anEntity, const KC_FloatRect& aBoundingRect);

private:
    constexpr std::int32_t GetOneDimensionCellsCount() const;

    const std::int32_t myGridCellSize;
    std::unordered_map<std::int32_t, KC_EntitySet> myGridCells;
};
