#include "DamageIndicator.h"

DamageIndicator::DamageIndicator(sf::Vector2f position, int damage, bool isPlayerDamage, const sf::Font& font)
    : lifetime(1.0f), timeElapsed(0.0f)
{
    text.setFont(font);
    text.setString("-" + std::to_string(damage));
    text.setCharacterSize(20);
    // Center the origin for easier positioning
    text.setOrigin(text.getLocalBounds().width / 2.0f, text.getLocalBounds().height / 2.0f);
    text.setPosition(position);

    // Set color and velocity based on who took the damage
    if (isPlayerDamage) {
        color = sf::Color::Red;     // Player was hit
        velocity = sf::Vector2f(0.f, -40.f); // Float up quickly (more urgency)
    }
    else {
        color = sf::Color::White;   // Monster was hit
        velocity = sf::Vector2f(0.f, -20.f); // Float up slowly
    }

    text.setFillColor(color);
}

// Returns true if the indicator is dead (lifetime expired)
bool DamageIndicator::update(float dt)
{
    timeElapsed += dt;

    // Move the text upwards
    text.move(velocity * dt);

    // Fade the text out near the end of its life
    float alpha = 255.0f * (1.0f - (timeElapsed / lifetime));
    if (alpha < 0) alpha = 0;

    sf::Color currentColor = color;
    // Set the alpha (transparency) channel
    currentColor.a = static_cast<sf::Uint8>(alpha);
    text.setFillColor(currentColor);

    // Check if lifetime has expired
    return timeElapsed >= lifetime;
}

void DamageIndicator::draw(sf::RenderWindow& window) const
{
    window.draw(text);
}