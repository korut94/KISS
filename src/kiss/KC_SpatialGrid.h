#pragma once

#include "KC_EntitySet.h"
#include "KC_Rect.h"

#include <SFML/System/Vector2.hpp>

#include <vector>
#include <unordered_map>

class KC_SpatialGrid final
{
public:
    KC_SpatialGrid(std::int32_t aGridCellSize);

    std::int32_t AverageEntityCountInGridCells() const;
    std::int32_t GetGridCellSize() const { return myGridCellSize; }
    sf::Vector2i GetGridCoordinate(std::uint32_t anIndex) const;
    void GetGridCoordinates(std::vector<sf::Vector2i>& outSomeGridCoordinates) const;
    void GetIndexs(std::vector<std::int32_t>& outSomeIndexes) const;
    std::int32_t MinEntitiesCountInGridCells() const;
    std::int32_t MaxEntitiesCountInGridCells() const;

    void Clear();
    void InsertEntity(KC_Entity anEntity, const KC_FloatRect& aBoundingRect);

private:
    sf::Vector2i GetGridCoordinate(sf::Vector2f aPosition) const;
    std::uint32_t GetIndex(sf::Vector2i aGridCoordinate) const;

    const std::int32_t myGridCellSize;
    std::unordered_map<std::uint32_t, std::vector<KC_Entity>> myGridCells;
};

#if IS_IMGUI
void KC_ImGui(KC_SpatialGrid& aSpatialGrid);
#endif // IS_IMGUI
