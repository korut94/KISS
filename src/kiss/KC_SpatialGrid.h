#pragma once

#include "KC_Assert.h"
#include "KC_EntitySet.h"
#include "KC_Rect.h"

#include <SFML/System/Vector2.hpp>

#include <array>
#include <vector>
#include <unordered_map>

class KC_SpatialGrid final
{
public:
    explicit KC_SpatialGrid(float aGridCellSize);

    void GetNearbyEntities(KC_Entity anEntity, std::vector<KC_Entity>& outSomeCloseEntities) const;
    float GetGridCellSize() const { return 1.f / myGridCellScale; }
    sf::Vector2i GetGridCoordinate(int32_t anHash) const;
    void GetGridCoordinates(std::vector<sf::Vector2i>& outSomeGridCoordinates) const;
#if IS_DEBUG_BUILD
    float Density() const;
    int32_t MaxEntitiesCountInGridCells() const;
    int32_t MinEntitiesCountInGridCells() const;
#endif // IS_DEBUG_BUILD

    template <typename TFunc>
    void ForEachCell(const KC_FloatRect& aBoundingRect, TFunc&& aFunction) const;
    template <typename TFunc>
    void ForEachCell(const KC_FloatRect& aBoundingRect, TFunc&& aFunction);

    void Clear();
    void InsertEntity(KC_Entity anEntity, const KC_FloatRect& aBoundingRect);
    void UpdateEntity(KC_Entity anEntity, const KC_FloatRect& aBoundingRect);

private:
    template <typename TFunc>
    void ForEachCellUnsafe(const KC_FloatRect& aBoundingRect, TFunc&& aFunction) const;

    sf::Vector2i GetGridCoordinate(sf::Vector2f aPosition) const;
    int32_t GetHash(sf::Vector2i aGridCoordinate) const;

    void InsertEntityInGrid(KC_Entity anEntity);
    void RemoveEntityFromGrid(KC_Entity anEntity);

    const float myGridCellScale;
    std::unordered_map<int32_t, std::vector<KC_Entity>> myGridCells;

    KC_EntitySet myEntitySet;
    std::vector<KC_FloatRect> myEntityBounds;
};

#if IS_IMGUI
void KC_ImGui(const KC_SpatialGrid& aSpatialGrid);
#endif // IS_IMGUI

template <typename TFunc>
void KC_SpatialGrid::ForEachCell(const KC_FloatRect& aBoundingRect, TFunc&& aFunction) const
{
    // See https://miro.com/app/board/uXjVL3mClFw=/?moveToWidget=3458764612608170994&cot=14
    const sf::Vector2i topLeft = GetGridCoordinate(aBoundingRect.GetTopLeft());
    const sf::Vector2i bottomRight = GetGridCoordinate(aBoundingRect.GetBottomRight());
    const sf::Vector2i distance = (bottomRight - topLeft) + sf::Vector2i(1, 1);

    for (int32_t index = 0, count = distance.x * distance.y; index < count; ++index)
    {
        const sf::Vector2i offset = { index % distance.x, index / distance.x };

        const auto itr = myGridCells.find(GetHash(topLeft + offset));
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
    const sf::Vector2i topLeft = GetGridCoordinate(aBoundingRect.GetTopLeft());
    const sf::Vector2i bottomRight = GetGridCoordinate(aBoundingRect.GetBottomRight());
    const sf::Vector2i distance = (bottomRight - topLeft) + sf::Vector2i(1, 1);

    for (int32_t index = 0, count = distance.x * distance.y; index < count; ++index)
    {
        const sf::Vector2i offset = { index % distance.x, index / distance.x };
        const int32_t hash = GetHash(topLeft + offset);

        aFunction(myGridCells[hash], hash);
    }
}

template <typename TFunc>
void KC_SpatialGrid::ForEachCellUnsafe(const KC_FloatRect& aBoundingRect, TFunc&& aFunction) const
{
    const sf::Vector2i topLeft = GetGridCoordinate(aBoundingRect.GetTopLeft());
    const sf::Vector2i bottomRight = GetGridCoordinate(aBoundingRect.GetBottomRight());
    const sf::Vector2i distance = (bottomRight - topLeft) + sf::Vector2i(1, 1);

    for (int32_t index = 0, count = distance.x * distance.y; index < count; ++index)
    {
        const sf::Vector2i offset = { index % distance.x, index / distance.x };
        // By profiling, it looks faster than the find function
        aFunction(myGridCells.at(GetHash(topLeft + offset)));
    }
}
