#pragma once

// We need the map constants declared in the utility header
// so that any file including Utility.h knows the map size.
const int MAP_WIDTH = 25;
const int MAP_HEIGHT = 18;
const int TILE_SIZE = 32;

// DECLARATIONS for the helper functions
bool isWall(int mapX, int mapY, const int map[MAP_HEIGHT][MAP_WIDTH]);
int getTileType(int mapX, int mapY, const int map[MAP_HEIGHT][MAP_WIDTH]);