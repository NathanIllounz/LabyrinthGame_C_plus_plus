#pragma once
#include "GameObject.h"
#include <iostream>

class Character : public GameObject
{
public:
    // Constructor passes arguments up to GameObject
    Character(float x, float y, float size, int initialHealth);
    virtual ~Character() = default;

    // Core Combat Methods
    void takeDamage(int damage);
    int getHealth() const;
    bool isAlive() const;
    bool isInvincible() const { return m_isInvincible; }

    // Movement/Physics (Virtual update to be overridden by Player/Monster)
    virtual void update(float dt, const int map[MAP_HEIGHT][MAP_WIDTH]) override = 0;

protected:
    int health;
    int maxHealth;
    bool m_isInvincible;
    sf::Clock damageTimer;

    // NEW: Handles the flickering/invincibility time after a hit
    void updateInvincibility(float dt);
};