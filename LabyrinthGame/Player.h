#pragma once
#include "Character.h" // CHANGE: Now inherits from Character

class Player : public Character // CHANGE: Inherit from Character
{
public:
    // Constructor (Must now pass initial health to Character)
    Player(float startX, float startY, float size, int initialHealth);

    // Override the base class update method
    void update(float dt, const int map[MAP_HEIGHT][MAP_WIDTH]) override;

private:
    float speed;
};

// DECLARATION of the collision check utility
bool isWall(int mapX, int mapY, const int map[MAP_HEIGHT][MAP_WIDTH]);