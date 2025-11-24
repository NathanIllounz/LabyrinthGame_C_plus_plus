#pragma once
#include "Character.h" // Now inherits from Character
#include <SFML/Window/Keyboard.hpp>
#include <SFML/System/Clock.hpp> // Needed for sf::Clock

class Player : public Character // Inherit from Character
{
public:
    // Constructor (Must pass initial health to Character)
    Player(float startX, float startY, float size, int initialHealth);

    // Override the base class update method
    void update(float dt, const int map[MAP_HEIGHT][MAP_WIDTH]) override;

    // --- NEW Public Getters for Attack Logic (Used by main.cpp) ---
    sf::Clock& getAttackTimer();
    float getAttackCooldown() const;
    float getAttackRange() const;
    int getAttackDamage() const;

private:
    float speed;

    // Attack Variables
    sf::Clock attackTimer;     // For cooldown tracking
    float attackCooldown;
    float attackRange;
    int attackDamage;
};

// DECLARATION of the collision check utility
bool isWall(int mapX, int mapY, const int map[MAP_HEIGHT][MAP_WIDTH]);