#pragma once
#include <SFML/Graphics.hpp>
#include "Utility.h"

class GameObject
{
public:
    // Constructor: Takes initial position and tile size
    GameObject(float x, float y, float size);

    // Virtual Destructor (Crucial for proper cleanup with inheritance)
    virtual ~GameObject() = default;

    // --- Core Methods ---

    // Pure virtual function: Must be implemented by derived classes (Player, Monster)
    // We pass the map array so the object can check for walls.
    virtual void update(float dt, const int map[MAP_HEIGHT][MAP_WIDTH]) = 0;

    // Renders the object
    virtual void draw(sf::RenderWindow& window);

    sf::FloatRect getBounds() const;

    // Returns the current position
    sf::Vector2f getPosition() const;

protected:
    // SFML shape used to represent the object visually
    sf::RectangleShape shape;
    float tileSize;
};