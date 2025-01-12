# Implementation note

## Spatial Grid
### First version:
- `key = gridCoordinate.x + gridCoordinate.y * #virtualCellsInRow`
- accepting negative coordinates
### Problems:
- Need to offset the world position by `(-gridCellSize / 2, -gridCellSize / 2)` otherwise, at the center, the distance to the next cell would be half
```
[-7.5, -2.5], ]-2.5, 0] ]0, 2.5], ]2.5, 7.5]
```
- after offset, handle the 0 and -0
``` 
]-5, 0][0, 5[ => -0 0...
```
- Idea: remove all the 0 x and y axis... it would make too complicated the indexing logic when I can offset by `(-gridCellSize / 2, -gridCellSize / 2)` to have always positive values
### Second version:
- `key = gridCoordinate.x + gridCoordinate.y * #virtualCellsInRow`
- offset the world position by `(-gridCellSize / 2, -gridCellSize / 2)`
### Problems:
- The max value for a `gridCoordinate` is `sqrt(INT_MAX) = 46340`, otherwise `gridCoordinate.y * #virtualCellsInRow` will go in overflow.
- Idea: `46340` is less than `2^16`, therefore composing the key by putting 16 bits for x and 16 bits for y is just better!
### Third version:
- `key = x << 16 | y`
- It might be interesting adding a factor scale in order to have spatial grid for meters and centimeters.
### Additional Notes
- Instead of a `KC_EntitySet`, moved to `std::vector` since it's faster to create (we save the sorting time) and we cant assume about the entity id distribution 
