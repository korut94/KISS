#pragma once

#include "KC_EntitySet.h"
#include "KC_Rect.h"

#include <SFML/System/Vector2.hpp>

#include <array>
#include <vector>
#include <unordered_map>

class KC_SpatialGrid final
{
public:
    KC_SpatialGrid(float aGridCellSize);

    std::int32_t AverageEntityCountInGridCells() const;
    void GetEntitiesInsideBound(const KC_FloatRect& aBoundingRect, std::vector<KC_Entity>& outSomeEntities) const;
    float GetGridCellSize() const { return 1.f / myGridCellScale; }
    sf::Vector2i GetGridCoordinate(std::int32_t anIndex) const;
    void GetGridCoordinates(std::vector<sf::Vector2i>& outSomeGridCoordinates) const;
    void GetIndexs(std::vector<std::int32_t>& outSomeIndexes) const;
    std::int32_t MinEntitiesCountInGridCells() const;
    std::int32_t MaxEntitiesCountInGridCells() const;

    void Clear();
    void InsertEntity(KC_Entity anEntity, const KC_FloatRect& aBoundingRect);

    template <typename TFunc>
    void ForEachCell(const KC_FloatRect& aBoundingRect, TFunc&& aFunction) const;
    template <typename TFunc>
    void ForEachCell(const KC_FloatRect& aBoundingRect, TFunc&& aFunction);

private:
    sf::Vector2i GetGridCoordinate(sf::Vector2f aPosition) const;
    std::int32_t GetIndex(sf::Vector2i aGridCoordinate) const;

    const float myGridCellScale;
    std::unordered_map<std::int32_t, std::vector<KC_Entity>> myGridCells;
};

#if IS_IMGUI
void KC_ImGui(KC_SpatialGrid& aSpatialGrid);
#endif // IS_IMGUI

template <typename TFunc>
void KC_SpatialGrid::ForEachCell(const KC_FloatRect& aBoundingRect, TFunc&& aFunction) const
{
    // See https://miro.com/app/board/uXjVL3mClFw=/?moveToWidget=3458764612608170994&cot=14
    const std::array<sf::Vector2i, 3> cornerGridCoorindate =
    { 
        GetGridCoordinate(aBoundingRect.GetTopLeft()),
        GetGridCoordinate(aBoundingRect.GetTopRight()),
        GetGridCoordinate(aBoundingRect.GetBottomLeft())
    };

    const sf::Vector2i distance =
    {
        cornerGridCoorindate[1].x - cornerGridCoorindate[0].x + 1,
        cornerGridCoorindate[2].y - cornerGridCoorindate[0].y + 1
    };

    for (std::int32_t index = 0, count = distance.x * distance.y; index < count; ++index)
    {
        const sf::Vector2i offset = { index % distance.x, index / distance.x };

        auto itr = myGridCells.find(GetIndex(cornerGridCoorindate[0] + offset));
        if (itr != myGridCells.cend())
        {
            aFunction(itr->second);
        }
    }
}

template <typename TFunc>
void KC_SpatialGrid::ForEachCell(const KC_FloatRect& aBoundingRect, TFunc&& aFunction)
{
    // See https://miro.com/app/board/uXjVL3mClFw=/?moveToWidget=3458764612608170994&cot=14
    const std::array<sf::Vector2i, 3> cornerGridCoorindate =
    { 
        GetGridCoordinate(aBoundingRect.GetTopLeft()),
        GetGridCoordinate(aBoundingRect.GetTopRight()),
        GetGridCoordinate(aBoundingRect.GetBottomLeft())
    };

    const sf::Vector2i distance =
    {
        cornerGridCoorindate[1].x - cornerGridCoorindate[0].x + 1,
        cornerGridCoorindate[2].y - cornerGridCoorindate[0].y + 1
    };

    for (std::int32_t index = 0, count = distance.x * distance.y; index < count; ++index)
    {
        const sf::Vector2i offset = { index % distance.x, index / distance.x };
        aFunction(myGridCells[GetIndex(cornerGridCoorindate[0] + offset)]);
    }
}
