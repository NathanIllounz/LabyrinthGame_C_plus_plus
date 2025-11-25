#include "Monster.h"
#include "Utility.h"
#include <cstdlib> 
#include <ctime>   

// Constructor implementation
Monster::Monster(float startX, float startY, float size, const int map[MAP_HEIGHT][MAP_WIDTH], int initialHealth)
// CHANGE: Call Character constructor with initialHealth
    : Character(startX, startY, size, initialHealth), speed(100.f), directionTimer(0.f)
{
    shape.setFillColor(sf::Color(150, 0, 0)); // Dark Red color
    // --- Spawn Validation Logic --- (REMAINS THE SAME)
    float currentX = startX;
    float currentY = startY;

    // Loop until we find a tile that is NOT a wall (type != 1)
    while (getTileType(static_cast<int>(currentX), static_cast<int>(currentY), map) == 1)
    {
        // Pick a new random position
        currentX = 1.5f + (std::rand() % (MAP_WIDTH - 2));
        currentY = 1.5f + (std::rand() % (MAP_HEIGHT - 2));
    }

    shape.setPosition(currentX * tileSize, currentY * tileSize);
}

void Monster::update(float dt, const int map[MAP_HEIGHT][MAP_WIDTH])
{
    // --- Update Invincibility Status ---
    updateInvincibility(dt);

    // If not invincible, set the monster's final color
    if (!m_isInvincible)
    {
        shape.setFillColor(sf::Color(150, 0, 0)); // Dark Red color
    }

    // If the monster is defeated, stop movement
    if (!isAlive())
    {
        shape.setFillColor(sf::Color::Black); // Dead monster is black
        return; // Stop update logic
    }

    // --- Movement ---
    moveRandomly(dt, map);
}
// ... (moveRandomly function REMAINS THE SAME)
void Monster::moveRandomly(float dt, const int map[MAP_HEIGHT][MAP_WIDTH])
{
    directionTimer += dt;

    // Change direction every 1.5 seconds (or if we hit a wall)
    if (directionTimer >= 1.5f)
    {
        directionTimer = 0.f;
        // Generate a random direction (0=Up, 1=Down, 2=Left, 3=Right, 4=Stop)
        int randomDirection = std::rand() % 5;

        currentDirection = sf::Vector2f(0.f, 0.f);
        if (randomDirection == 0) currentDirection.y = -1.f;
        else if (randomDirection == 1) currentDirection.y = 1.f;
        else if (randomDirection == 2) currentDirection.x = -1.f;
        else if (randomDirection == 3) currentDirection.x = 1.f;
        // If randomDirection is 4, it remains (0, 0) and the monster stops momentarily
    }

    sf::Vector2f movement = currentDirection * speed * dt;

    // --- Collision Check (Simplified for monster AI) ---
    sf::Vector2f nextPos = shape.getPosition() + movement;
    int checkTileX = static_cast<int>(nextPos.x / TILE_SIZE);
    int checkTileY = static_cast<int>(nextPos.y / TILE_SIZE);

    if (!isWall(checkTileX, checkTileY, map)) {
        shape.move(movement);
    }
    else {
        // If the monster hits a wall, reset the timer to choose a new direction immediately
        directionTimer = 1.5f;
    }
}