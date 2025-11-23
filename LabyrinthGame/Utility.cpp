#include "Utility.h"

// DEFINITION: Checks if the tile at (mapX, mapY) is a wall (1)
bool isWall(int mapX, int mapY, const int map[MAP_HEIGHT][MAP_WIDTH])
{
    // Basic Bounds Check: Treat outside the map as a wall
    if (mapY < 0 || mapY >= MAP_HEIGHT || mapX < 0 || mapX >= MAP_WIDTH) {
        return true;
    }
    // Check the array value: If it's 1, it's a wall.
    return map[mapY][mapX] == 1;
}

// DEFINITION: Returns the tile value (0=Floor, 1=Wall, 2=Exit, etc.)
int getTileType(int mapX, int mapY, const int map[MAP_HEIGHT][MAP_WIDTH])
{
    // Basic Bounds Check: Treat outside the map as a wall (1)
    if (mapY < 0 || mapY >= MAP_HEIGHT || mapX < 0 || mapX >= MAP_WIDTH) {
        return 1;
    }
    // Return the value from the map array
    return map[mapY][mapX];
}