#include "GameObject.h"
#include "Utility.h"

// Constructor implementation
// Note: x and y are passed as tile coordinates (e.g., 1.5, 1.5)
GameObject::GameObject(float x, float y, float size) : tileSize(size)
{
    // Initialize the shape (80% of a tile size)
    shape.setSize(sf::Vector2f(tileSize * 0.8f, tileSize * 0.8f));
    shape.setOrigin(shape.getSize() / 2.f); // Set origin to center for easier positioning
    // Convert tile coordinates to screen pixels
    shape.setPosition(x * tileSize, y * tileSize);
}

// Bounding box accessor implementation (NEW)
sf::FloatRect GameObject::getBounds() const
{
    // Returns the global boundaries of the SFML shape, which is used for collision
    return shape.getGlobalBounds();
}

// Drawing implementation
void GameObject::draw(sf::RenderWindow& window)
{
    window.draw(shape);
}

// Position accessor
sf::Vector2f GameObject::getPosition() const
{
    return shape.getPosition();
}