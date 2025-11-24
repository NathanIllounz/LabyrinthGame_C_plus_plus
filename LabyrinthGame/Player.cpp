#include "Player.h"
#include "Utility.h" // For isWall
#include <SFML/Window/Keyboard.hpp>
#include <iostream>

// The isWall function definition should be in Utility.cpp, but declared here for safety
extern bool isWall(int mapX, int mapY, const int map[MAP_HEIGHT][MAP_WIDTH]);

// Constructor calls the base class (Character) constructor and sets initial health/attack stats
Player::Player(float startX, float startY, float size, int initialHealth)
    : Character(startX, startY, size, initialHealth), speed(150.f),
    attackCooldown(0.5f), attackRange(TILE_SIZE * 1.5f), attackDamage(1)
{
    shape.setFillColor(sf::Color::Red);
}

void Player::update(float dt, const int map[MAP_HEIGHT][MAP_WIDTH])
{
    // --- Update Invincibility Status (Inherited from Character) ---
    updateInvincibility(dt);

    // If not invincible, set the player's final color
    if (!isInvincible)
    {
        shape.setFillColor(sf::Color::Red);
    }

    // If defeated, stop processing
    if (!isAlive())
    {
        return;
    }

    // --- Movement and Collision Logic ---
    sf::Vector2f movement(0.f, 0.f);

    // Input Handling (WASD)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        movement.y -= speed * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        movement.y += speed * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        movement.x -= speed * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        movement.x += speed * dt;

    // Collision Checks (X and Y axis)
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

    // --- Attack Input Check (Handled in main.cpp for hit detection) ---
    // The cooldown check is still done here for visual confirmation/sound in a full game
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) &&
        attackTimer.getElapsedTime().asSeconds() >= attackCooldown)
    {
        // Attack is initiated. main.cpp handles the target check.
        // We only restart the timer here.
        attackTimer.restart();
    }
}

// --- NEW Getters Implementation ---
sf::Clock& Player::getAttackTimer()
{
    return attackTimer;
}

float Player::getAttackCooldown() const
{
    return attackCooldown;
}

float Player::getAttackRange() const
{
    return attackRange;
}

int Player::getAttackDamage() const
{
    return attackDamage;
}