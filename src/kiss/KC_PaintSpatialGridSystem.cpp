#include "KC_PaintSpatialGridSystem.h"

#include "KC_Assert.h"
#include "KC_Canvas.h"
#include "KC_SpatialGrid.h"
#include "KC_SpatialGridPalette.h"

void KC_PaintSpatialGridSystem::Run()
{
    for (KC_Entity entity : myEntitySet)
    {
        KC_Canvas& canvas = GetCanvas(entity);
        const KC_SpatialGridPalette& palette = GetComponent<KC_SpatialGridPalette>(entity);
        const KC_SpatialGrid& spatialGrid = palette.mySpatialGrid;

        std::vector<sf::Vector2i> gridCoordinates;
        spatialGrid.GetGridCoordinates(gridCoordinates);

        const std::int32_t gridCellSize = spatialGrid.GetGridCellSize();
        const float gridCellSideLenght = static_cast<float>(gridCellSize) ;

        for (sf::Vector2i gridCoordinate : gridCoordinates)
        {
            const sf::Vector2f center = { gridCoordinate.x * gridCellSideLenght, gridCoordinate.y * gridCellSideLenght };
            const sf::Vector2f size = { gridCellSideLenght, gridCellSideLenght };

            DrawRectangle(canvas, center, size, sf::Color::Green);
        }
    }
}
