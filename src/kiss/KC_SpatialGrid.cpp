#include "KC_SpatialGrid.h"

#include "KC_Assert.h"

#if IS_IMGUI
#include "imgui.h"
#endif // IS_IMGUI

#include <algorithm>
#include <array>

namespace KC_SpatialGrid_Private
{
    static constexpr const std::uint32_t locCoordinateSize = 16;
    static constexpr const std::uint32_t locXShift = locCoordinateSize;
    static constexpr const std::uint32_t locYShit = 0;
    static constexpr const sf::Vector2f locOffset = { (1 << (locCoordinateSize - 1)) - 1, (1 << (locCoordinateSize - 1)) - 1 };

    bool IsWithinBounds(sf::Vector2i aGridCoordinate)
    {
        return (aGridCoordinate.x >> locCoordinateSize) == 0 && (aGridCoordinate.y >> locCoordinateSize) == 0;
    }

    std::uint32_t X(std::uint32_t anIndex)
    {
        return anIndex >> locXShift;
    }

    std::uint32_t Y(std::uint32_t anIndex)
    {
        return anIndex & ((1 << locXShift) - 1);
    }
}

KC_SpatialGrid::KC_SpatialGrid(std::int32_t aGridCellSize)
    : myGridCellSize(aGridCellSize)
{
    KC_ASSERT(aGridCellSize >= 1);
}

std::int32_t KC_SpatialGrid::AverageEntityCountInGridCells() const
{
    using Pair = std::pair<std::uint32_t, std::vector<KC_Entity>>;

    if (myGridCells.empty())
        return 0;

    std::int32_t totalEntitiesCount = 0;
    for (const Pair& pair : myGridCells)
    {
        totalEntitiesCount += pair.second.size();
    }

    return totalEntitiesCount / myGridCells.size();
}

sf::Vector2i KC_SpatialGrid::GetGridCoordinate(std::uint32_t anIndex) const
{
    namespace Private = KC_SpatialGrid_Private;
    return sf::Vector2i(Private::X(anIndex), Private::Y(anIndex)) - static_cast<sf::Vector2i>(Private::locOffset) / myGridCellSize;
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
    // See https://miro.com/app/board/uXjVL3mClFw=/?moveToWidget=3458764612608170994&cot=14
    namespace Private = KC_SpatialGrid_Private;

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
        myGridCells[GetIndex(cornerGridCoorindate[0] + offset)].push_back(anEntity);
    }
}

std::int32_t KC_SpatialGrid::MinEntitiesCountInGridCells() const
{
    using Pair = std::pair<std::uint32_t, std::vector<KC_Entity>>;
    auto itr = std::min_element(myGridCells.cbegin(), myGridCells.cend(), [](const Pair& a, const Pair& b)
    {
        return a.second.size() < b.second.size();
    });

    return itr != myGridCells.cend() ? itr->second.size() : 0;
}

std::int32_t KC_SpatialGrid::MaxEntitiesCountInGridCells() const
{
    using Pair = std::pair<std::uint32_t, std::vector<KC_Entity>>;
    auto itr = std::max_element(myGridCells.cbegin(), myGridCells.cend(), [](const Pair& a, const Pair& b)
    {
        return a.second.size() < b.second.size();
    });

    return itr != myGridCells.cend() ? itr->second.size() : 0;
}

sf::Vector2i KC_SpatialGrid::GetGridCoordinate(sf::Vector2f aPosition) const
{
    namespace Private = KC_SpatialGrid_Private;

    const sf::Vector2i gridCoordinate = static_cast<sf::Vector2i>(aPosition + Private::locOffset) / myGridCellSize;
    KC_ASSERT(Private::IsWithinBounds(gridCoordinate));

    return gridCoordinate;
}

std::uint32_t KC_SpatialGrid::GetIndex(sf::Vector2i aGridCoordinate) const
{
    namespace Private = KC_SpatialGrid_Private;
    return aGridCoordinate.x << Private::locXShift | aGridCoordinate.y << Private::locYShit;
}

#if IS_IMGUI
void KC_ImGui(KC_SpatialGrid& aSpatialGrid)
{
    const std::int32_t gridCellSize = aSpatialGrid.GetGridCellSize();

    ImGui::SeparatorText("Spatial Grid");
    std::vector<sf::Vector2i> gridCoordinates;
    aSpatialGrid.GetGridCoordinates(gridCoordinates);
    ImGui::Text("Cells Count: %d [%dx%d]", gridCoordinates.size(), gridCellSize, gridCellSize);
    ImGui::Text("Min Entities: %d", aSpatialGrid.MinEntitiesCountInGridCells());
    ImGui::Text("Max Entities: %d", aSpatialGrid.MaxEntitiesCountInGridCells());
    ImGui::Text("Avg Entities: %d", aSpatialGrid.AverageEntityCountInGridCells());
}
#endif // IS_IMGUI
