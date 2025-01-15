#pragma once

#include "KC_EntitySet.h"
#include "KC_Rect.h"

#include <SFML/System/Vector2.hpp>

#include <vector>
#include <unordered_map>

class KC_SpatialGrid final
{
public:
    KC_SpatialGrid(float aGridCellSize);

    std::int32_t AverageEntityCountInGridCells() const;
    float GetGridCellSize() const { return 1.f / myGridCellScale; }
    sf::Vector2i GetGridCoordinate(std::int32_t anIndex) const;
    void GetGridCoordinates(std::vector<sf::Vector2i>& outSomeGridCoordinates) const;
    void GetIndexs(std::vector<std::int32_t>& outSomeIndexes) const;
    std::int32_t MinEntitiesCountInGridCells() const;
    std::int32_t MaxEntitiesCountInGridCells() const;

    void Clear();
    void InsertEntity(KC_Entity anEntity, const KC_FloatRect& aBoundingRect);

private:
    sf::Vector2i GetGridCoordinate(sf::Vector2f aPosition) const;
    std::int32_t GetIndex(sf::Vector2i aGridCoordinate) const;

    const float myGridCellScale;
    std::unordered_map<std::int32_t, std::vector<KC_Entity>> myGridCells;
};

#if IS_IMGUI
void KC_ImGui(KC_SpatialGrid& aSpatialGrid);
#endif // IS_IMGUI
