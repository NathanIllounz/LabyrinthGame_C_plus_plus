#include "Character.h"

Character::Character(float x, float y, float size, int initialHealth)
    : GameObject(x, y, size), health(initialHealth), maxHealth(initialHealth), m_isInvincible(false)
{
}

void Character::takeDamage(int damage)
{
    if (!m_isInvincible)
    {
        health -= damage;
        std::cout << "Took " << damage << " damage. Health: " << health << std::endl;
        m_isInvincible = true;
        damageTimer.restart();
        shape.setFillColor(sf::Color::Yellow); // Flash yellow on hit
    }
}

void Character::updateInvincibility(float dt)
{
    // Check if invincibility period (1.5 seconds) is over
    if (m_isInvincible && damageTimer.getElapsedTime().asSeconds() > 1.5f)
    {
        m_isInvincible = false;
        // The derived class (Player/Monster) must set its final color in its update function
    }
    // Simple visual effect: make the character flash/flicker while invincible
    if (m_isInvincible)
    {
        // Toggle visibility quickly
        int time_ms = damageTimer.getElapsedTime().asMilliseconds();
        shape.setFillColor((time_ms / 100) % 2 == 0 ? sf::Color::Yellow : sf::Color::Transparent);
    }
}

int Character::getHealth() const
{
    return health;
}

bool Character::isAlive() const
{
    return health > 0;
}