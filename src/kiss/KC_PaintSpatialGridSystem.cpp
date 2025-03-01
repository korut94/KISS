#include "KC_PaintSpatialGridSystem.h"

#include "KC_Assert.h"
#include "KC_Canvas.h"
#include "KC_Math.h"
#include "KC_SpatialGrid.h"
#include "KC_SpatialGridPalette.h"

void KC_PaintSpatialGridSystem::Run()
{
    const std::vector<KC_SpatialGrid>& spatialGrids = GetSpatialGrids();

    for (size_t i = 0, count = myEntitySet.Count(); i < count; ++i)
    {
        const KC_Entity entity = myEntitySet.At(i);
        const KC_SpatialGrid& spatialGrid = spatialGrids[i];

        KC_Canvas& canvas = GetCanvas(entity);
        const KC_SpatialGridPalette& palette = GetComponent<KC_SpatialGridPalette>(entity);
        const KC_CanvasRectanglePalette cellPalette = { palette.myGridCellFillColor, palette.myGridLineColor, palette.myGridLineThickness };

        std::vector<sf::Vector2i> gridCoordinates;
        spatialGrid.GetGridCoordinates(gridCoordinates);

        const float gridCellSize = spatialGrid.GetGridCellSize();

        for (sf::Vector2i gridCoordinate : gridCoordinates)
        {   
            const sf::Vector2f offset = 
            {
                KC_Math::Sign(gridCoordinate.x) * gridCellSize * 0.5f,
                KC_Math::Sign(gridCoordinate.y) * gridCellSize * 0.5f
            };

            const sf::Vector2f center = sf::Vector2f(gridCoordinate.x * gridCellSize, gridCoordinate.y * gridCellSize) + offset;
            const sf::Vector2f size =
            {
                (gridCoordinate.x == 0 ? 2.f : 1.f) * gridCellSize, 
                (gridCoordinate.y == 0 ? 2.f : 1.f) * gridCellSize
            };
            
            DrawRectangle(canvas, center, size, cellPalette);
        }
    }
}
