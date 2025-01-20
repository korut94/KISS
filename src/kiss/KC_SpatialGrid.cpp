#include "KC_SpatialGrid.h"

#include "KC_Assert.h"

#if IS_IMGUI
#include "imgui.h"
#endif // IS_IMGUI

#include <algorithm>

namespace KC_SpatialGrid_Private
{
    static constexpr const std::uint32_t locXShift = 16;
    static constexpr const std::uint32_t locYMask = (1 << locXShift) - 1;

    std::int32_t X(std::int32_t anIndex)
    {
        return anIndex >> locXShift;
    }

    std::int32_t Y(std::int32_t anIndex)
    {
        return static_cast<std::int16_t>(anIndex & locYMask);
    }
}

KC_SpatialGrid::KC_SpatialGrid(float aGridCellSize)
    : myGridCellScale(1.f / aGridCellSize)
{
}

std::int32_t KC_SpatialGrid::AverageEntityCountInGridCells() const
{
    using Pair = std::pair<std::int32_t, std::vector<KC_Entity>>;

    if (myGridCells.empty())
        return 0;

    std::int32_t totalEntitiesCount = 0;
    for (const Pair& pair : myGridCells)
    {
        totalEntitiesCount += pair.second.size();
    }

    return totalEntitiesCount / myGridCells.size();
}

void KC_SpatialGrid::GetEntitiesInsideBound(const KC_FloatRect& aBoundingRect, std::vector<KC_Entity>& outSomeEntities) const
{
    ForEachCell(aBoundingRect, [&outSomeEntities](const std::vector<KC_Entity>& someEntitiesInCell)
    {
        for (KC_Entity entity : someEntitiesInCell)
        {
            // No duplicates
            auto itr = std::find(outSomeEntities.cbegin(), outSomeEntities.cend(), entity);
            if (itr == outSomeEntities.cend())
            {
                outSomeEntities.push_back(entity);
            }
        }
    });
}

sf::Vector2i KC_SpatialGrid::GetGridCoordinate(std::int32_t anIndex) const
{
    namespace Private = KC_SpatialGrid_Private;
    return sf::Vector2i(Private::X(anIndex), Private::Y(anIndex));
}

void KC_SpatialGrid::GetGridCoordinates(std::vector<sf::Vector2i>& outSomeGridCoordinates) const
{
    outSomeGridCoordinates.reserve(myGridCells.size());

    for (auto itr = myGridCells.cbegin(), end = myGridCells.cend(); itr != end; ++itr)
    {
        outSomeGridCoordinates.push_back(GetGridCoordinate(itr->first));
    }
}

void KC_SpatialGrid::GetIndexs(std::vector<std::int32_t>& outSomeIndexs) const
{
    outSomeIndexs.reserve(myGridCells.size());

    for (auto itr = myGridCells.cbegin(), end = myGridCells.cend(); itr != end; ++itr)
    {
        outSomeIndexs.push_back(itr->first);
    }
}

void KC_SpatialGrid::Clear()
{
    myGridCells.clear();
}

void KC_SpatialGrid::InsertEntity(KC_Entity anEntity, const KC_FloatRect& aBoundingRect)
{
    ForEachCell(aBoundingRect, [anEntity](std::vector<KC_Entity>& someEntitiesInCell)
    {
        someEntitiesInCell.push_back(anEntity);
    });
}

std::int32_t KC_SpatialGrid::MinEntitiesCountInGridCells() const
{
    using Pair = std::pair<std::int32_t, std::vector<KC_Entity>>;
    auto itr = std::min_element(myGridCells.cbegin(), myGridCells.cend(), [](const Pair& a, const Pair& b)
    {
        return a.second.size() < b.second.size();
    });

    return itr != myGridCells.cend() ? itr->second.size() : 0;
}

std::int32_t KC_SpatialGrid::MaxEntitiesCountInGridCells() const
{
    using Pair = std::pair<std::int32_t, std::vector<KC_Entity>>;
    auto itr = std::max_element(myGridCells.cbegin(), myGridCells.cend(), [](const Pair& a, const Pair& b)
    {
        return a.second.size() < b.second.size();
    });

    return itr != myGridCells.cend() ? itr->second.size() : 0;
}

sf::Vector2i KC_SpatialGrid::GetGridCoordinate(sf::Vector2f aPosition) const
{
    namespace Private = KC_SpatialGrid_Private;
    return static_cast<sf::Vector2i>(aPosition * myGridCellScale);
}

std::int32_t KC_SpatialGrid::GetIndex(sf::Vector2i aGridCoordinate) const
{
    namespace Private = KC_SpatialGrid_Private;
    return (aGridCoordinate.x << Private::locXShift) | (aGridCoordinate.y & Private::locYMask);
}

#if IS_IMGUI
void KC_ImGui(KC_SpatialGrid& aSpatialGrid)
{
    const float gridCellSize = aSpatialGrid.GetGridCellSize();

    ImGui::SeparatorText("Spatial Grid");
    std::vector<sf::Vector2i> gridCoordinates;
    aSpatialGrid.GetGridCoordinates(gridCoordinates);
    ImGui::Text("Cells: %d [%.1fx%.1f]", gridCoordinates.size(), gridCellSize, gridCellSize);
    ImGui::Text("Min Entities: %d", aSpatialGrid.MinEntitiesCountInGridCells());
    ImGui::Text("Max Entities: %d", aSpatialGrid.MaxEntitiesCountInGridCells());
    ImGui::Text("Avg Entities: %d", aSpatialGrid.AverageEntityCountInGridCells());
}
#endif // IS_IMGUI
