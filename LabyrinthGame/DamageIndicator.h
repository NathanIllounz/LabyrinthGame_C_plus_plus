#pragma once
#include <SFML/Graphics.hpp>

class DamageIndicator
{
public:
    // Constructor: Takes position, damage value, and the font object
    DamageIndicator(sf::Vector2f position, int damage, bool isPlayerDamage, const sf::Font& font);

    // Updates the text position/fade over time. Returns true if the indicator is dead.
    bool update(float dt);

    // Draws the text to the window
    void draw(sf::RenderWindow& window) const;

private:
    sf::Text text;
    sf::Color color;
    float lifetime;     // Time the text remains visible (e.g., 1.0 second)
    float timeElapsed;  // How long the text has been active
    sf::Vector2f velocity; // For making the text float up
};