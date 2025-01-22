#include "KC_SpatialGrid.h"

#include "KC_Assert.h"
#include "KC_Profiling.h"

#if IS_IMGUI
#include "imgui.h"
#endif // IS_IMGUI

#include <algorithm>

namespace KC_SpatialGrid_Private
{
    static constexpr const uint32_t locXShift = 16;
    static constexpr const uint32_t locYMask = (1 << locXShift) - 1;

    int32_t X(int32_t anHash)
    {
        return anHash >> locXShift;
    }

    int32_t Y(int32_t anHash)
    {
        return static_cast<int16_t>(anHash & locYMask);
    }
}

KC_SpatialGrid::KC_SpatialGrid(float aGridCellSize)
    : myGridCellScale(1.f / aGridCellSize)
{
}

int32_t KC_SpatialGrid::AverageEntityCountInGridCells() const
{
    using Pair = std::pair<int32_t, std::vector<KC_Entity>>;

    if (myGridCells.empty())
        return 0;

    int32_t totalEntitiesCount = 0;
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

sf::Vector2i KC_SpatialGrid::GetGridCoordinate(int32_t anHash) const
{
    namespace Private = KC_SpatialGrid_Private;
    return sf::Vector2i(Private::X(anHash), Private::Y(anHash));
}

void KC_SpatialGrid::GetGridCoordinates(std::vector<sf::Vector2i>& outSomeGridCoordinates) const
{
    outSomeGridCoordinates.reserve(myGridCells.size());

    for (auto itr = myGridCells.cbegin(), end = myGridCells.cend(); itr != end; ++itr)
    {
        outSomeGridCoordinates.push_back(GetGridCoordinate(itr->first));
    }
}

void KC_SpatialGrid::Clear()
{
    myGridCells.clear();
}

void KC_SpatialGrid::InsertEntity(KC_Entity anEntity, const KC_FloatRect& aBoundingRect)
{
    KC_ASSERT(!myEntitySet.Contains(anEntity), "Entity already in the spatial grid");

    myEntitySet.Insert(anEntity);
    myEntityBounds.emplace_back(aBoundingRect);

    InsertEntityInGrid(anEntity);
}

void KC_SpatialGrid::UpdateEntity(KC_Entity anEntity, const KC_FloatRect& aBoundingRect)
{
    KC_ASSERT(myEntitySet.Contains(anEntity), "Entity not in the spatial grid");

    const KC_EntitySet::EntityIndex index = myEntitySet.GetIndex(anEntity);

    KC_FloatRect& entityBound = myEntityBounds[index];
    const sf::Vector2i currentTopLeft = GetGridCoordinate(entityBound.GetTopLeft());
    const sf::Vector2i currentBottomRight = GetGridCoordinate(entityBound.GetBottomRight());

    const sf::Vector2i newTopLeft = GetGridCoordinate(aBoundingRect.GetTopLeft());
    const sf::Vector2i newBottomRight = GetGridCoordinate(aBoundingRect.GetBottomRight());

    if (newTopLeft == currentTopLeft && newBottomRight == currentBottomRight)
        return;

    // Remove the entity with its previous bound
    RemoveEntityFromGrid(anEntity);

    entityBound = aBoundingRect;
    InsertEntityInGrid(anEntity);
}

int32_t KC_SpatialGrid::MinEntitiesCountInGridCells() const
{
    using Pair = std::pair<int32_t, std::vector<KC_Entity>>;
    auto itr = std::min_element(myGridCells.cbegin(), myGridCells.cend(), [](const Pair& a, const Pair& b)
    {
        return a.second.size() < b.second.size();
    });

    return itr != myGridCells.cend() ? itr->second.size() : 0;
}

int32_t KC_SpatialGrid::MaxEntitiesCountInGridCells() const
{
    using Pair = std::pair<int32_t, std::vector<KC_Entity>>;
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

int32_t KC_SpatialGrid::GetHash(sf::Vector2i aGridCoordinate) const
{
    namespace Private = KC_SpatialGrid_Private;
    return (aGridCoordinate.x << Private::locXShift) | (aGridCoordinate.y & Private::locYMask);
}

void KC_SpatialGrid::InsertEntityInGrid(KC_Entity anEntity)
{
    const KC_EntitySet::EntityIndex index = myEntitySet.GetIndex(anEntity);
    ForEachCell(myEntityBounds[index], [anEntity](std::vector<KC_Entity>& someEntitiesInCell, int32_t /*anHash*/)
    {
        someEntitiesInCell.push_back(anEntity);
    });
}

void KC_SpatialGrid::RemoveEntityFromGrid(KC_Entity anEntity)
{
    const KC_EntitySet::EntityIndex index = myEntitySet.GetIndex(anEntity);
    ForEachCell(myEntityBounds[index], [this, anEntity](std::vector<KC_Entity>& someEntitiesInCell, int32_t anHash)
    {
        auto itr = std::find(someEntitiesInCell.begin(), someEntitiesInCell.end(), anEntity);
        KC_ASSERT(itr != someEntitiesInCell.end());

        // Remove cyclic
        *itr = someEntitiesInCell.back();
        someEntitiesInCell.pop_back();

        if (someEntitiesInCell.size() == 0)
        {
            // Safe operation since ForEachCell looks-up at generated hash in each iteration
            myGridCells.erase(anHash);
        }
    });
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
