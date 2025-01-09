#include "KC_SpatialGrid.h"

#include "KC_Assert.h"

#include <array>
#include <limits>

namespace KC_SpatialGrid_Private
{
    static constexpr std::int32_t locMaxGridSide = std::numeric_limits<std::int32_t>::max();
    static constexpr std::int32_t locHalfMaxGridSide = locMaxGridSide >> 1;

    bool IsInsideGrid(const sf::Vector2f& aPosition)
    {
        return std::abs(static_cast<std::int32_t>(aPosition.x)) <= KC_SpatialGrid_Private::locHalfMaxGridSide
            && std::abs(static_cast<std::int32_t>(aPosition.y)) <= KC_SpatialGrid_Private::locHalfMaxGridSide;
    }
}

KC_SpatialGrid::KC_SpatialGrid(std::int32_t aCellGridSize)
    : myGridCellSize(aCellGridSize)
{
    KC_ASSERT(aCellGridSize > 0);
}

std::int32_t KC_SpatialGrid::GetIndex(const sf::Vector2f& aPosition) const
{
    KC_ASSERT(KC_SpatialGrid_Private::IsInsideGrid(aPosition));

    const sf::Vector2i gridCoordinate = static_cast<sf::Vector2i>(aPosition) / myGridCellSize;
    return gridCoordinate.x + gridCoordinate.y * GetOneDimensionCellsCount();
}

void KC_SpatialGrid::InsertEntity(KC_Entity anEntity, const KC_FloatRect& aBoundingRect)
{
    // See https://miro.com/app/board/uXjVL3mClFw=/?moveToWidget=3458764612608170994&cot=14
    const std::array<std::int32_t, 3> cellGridIndexs =
    { 
        GetIndex(aBoundingRect.GetTopLeft()),
        GetIndex(aBoundingRect.GetTopRight()),
        GetIndex(aBoundingRect.GetBottomLeft())
    };

    const std::int32_t subGridColumsCount = (cellGridIndexs[1] - cellGridIndexs[0]) + 1;
    const std::int32_t subGridRowsCount = ((cellGridIndexs[2] - cellGridIndexs[0]) / GetOneDimensionCellsCount()) + 1;

    for (std::int32_t index = 0, count = subGridRowsCount * subGridColumsCount; index < count; ++index)
    {
        const std::int32_t column = index % subGridColumsCount;
        const std::int32_t row = (index / subGridColumsCount) * GetOneDimensionCellsCount();

        myGridCells[row + cellGridIndexs[0] + column].Insert(anEntity);
    }
}

constexpr std::int32_t KC_SpatialGrid::GetOneDimensionCellsCount() const
{
    return KC_SpatialGrid_Private::locMaxGridSide / myGridCellSize;
}
