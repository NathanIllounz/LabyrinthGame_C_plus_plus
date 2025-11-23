#pragma once
#include "Character.h" // CHANGE: Now inherits from Character

class Monster : public Character // CHANGE: Inherit from Character
{
public:
    // Constructor (Must now pass initial health to Character)
    Monster(float startX, float startY, float size, const int map[MAP_HEIGHT][MAP_WIDTH], int initialHealth);

    // Override the base class update method for movement and AI
    void update(float dt, const int map[MAP_HEIGHT][MAP_WIDTH]) override;

    // Simple utility function to move the monster
    void moveRandomly(float dt, const int map[MAP_HEIGHT][MAP_WIDTH]);

private:
    float speed;
    float directionTimer;
    sf::Vector2f currentDirection;
};

// DECLARATION of utilities
bool isWall(int mapX, int mapY, const int map[MAP_HEIGHT][MAP_WIDTH]);
int getTileType(int mapX, int mapY, const int map[MAP_HEIGHT][MAP_WIDTH]);