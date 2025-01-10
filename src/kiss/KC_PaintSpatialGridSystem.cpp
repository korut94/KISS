#include "KC_PaintSpatialGridSystem.h"

#include "KC_Assert.h"
#include "KC_Canvas.h"
#include "KC_SpatialGrid.h"
#include "KC_SpatialGridPalette.h"

void KC_PaintSpatialGridSystem::Run()
{
    for (KC_Entity entity : myEntitySet)
    {
        KC_Canvas& canvas = GetComponent<KC_Canvas>(entity);
        const KC_SpatialGridPalette& palette = GetComponent<KC_SpatialGridPalette>(entity);
        const KC_SpatialGrid& spatialGrid = palette.mySpatialGrid;

        std::vector<sf::Vector2i> gridCoordinates;
        spatialGrid.GetGridCoordinates(gridCoordinates);

        const std::int32_t gridCellSize = spatialGrid.GetGridCellSize() * 2;

        for (sf::Vector2i gridCoordinate : gridCoordinates)
        {
            const float sideLength = static_cast<float>(gridCellSize);

            const sf::Vector2f center = static_cast<sf::Vector2f>(gridCoordinate * gridCellSize);
            const sf::Vector2f size = sf::Vector2f(sideLength, sideLength);

            DrawRectangle(canvas, center, size, sf::Color(255, 255 * (abs(gridCoordinate.y) % gridCellSize), 255 * (abs(gridCoordinate.x) % gridCellSize)));
        }
    }
}
