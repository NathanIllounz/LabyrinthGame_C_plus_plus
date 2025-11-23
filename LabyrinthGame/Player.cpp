#include "Player.h"
#include "Utility.h" // For isWall
#include <SFML/Window/Keyboard.hpp> 

// --- Helper Function Definition (Assuming isWall is now in Utility.cpp) ---
extern bool isWall(int mapX, int mapY, const int map[MAP_HEIGHT][MAP_WIDTH]);

// Constructor calls the base class (Character) constructor and sets initial health
Player::Player(float startX, float startY, float size, int initialHealth)
    : Character(startX, startY, size, initialHealth), speed(150.f) // Call Character constructor
{
    shape.setFillColor(sf::Color::Red);
}

void Player::update(float dt, const int map[MAP_HEIGHT][MAP_WIDTH])
{
    // --- Update Invincibility Status ---
    updateInvincibility(dt);

    // If not invincible, set the player's final color
    if (!isInvincible)
    {
        shape.setFillColor(sf::Color::Red);
    }

    // --- Movement and Collision Logic ---
    sf::Vector2f movement(0.f, 0.f);

    // Input Handling (WASD)
    // ... (Keep your WASD input checks here) ...
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        movement.y -= speed * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        movement.y += speed * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        movement.x -= speed * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        movement.x += speed * dt;
    // ... (Collision Checks follow, using shape.move()) ...

    // --- Collision Check (X-axis) ---
    sf::Vector2f nextPos_X = shape.getPosition() + sf::Vector2f(movement.x, 0.f);
    int checkTileX_X = static_cast<int>(nextPos_X.x / TILE_SIZE);
    int checkTileY_X = static_cast<int>(nextPos_X.y / TILE_SIZE);

    if (!isWall(checkTileX_X, checkTileY_X, map)) {
        shape.move(movement.x, 0.f);
    }

    // --- Collision Check (Y-axis) ---
    sf::Vector2f nextPos_Y = shape.getPosition() + sf::Vector2f(0.f, movement.y);
    int checkTileX_Y = static_cast<int>(nextPos_Y.x / TILE_SIZE);
    int checkTileY_Y = static_cast<int>(nextPos_Y.y / TILE_SIZE);

    if (!isWall(checkTileX_Y, checkTileY_Y, map)) {
        shape.move(0.f, movement.y);
    }

    // --- Game Over Check ---
    if (!isAlive())
    {
        std::cout << "Player has been defeated!" << std::endl;
    }
}
// DELETE the old takeDamage and health/timer related code.