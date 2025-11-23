#include <SFML/Graphics.hpp>
#include <iostream>
#include "Player.h"     // Includes Player class
#include "Monster.h"    // Includes Monster class
#include "Utility.h"    // Includes MAP_WIDTH, MAP_HEIGHT, and helper declarations

// The 2D array representing the fixed map
int fixedMap[MAP_HEIGHT][MAP_WIDTH] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,1,1,1,0,1,1,1,1,1,1,0,1,1,1,1,0,1,1,1,1,1,0,1},
    {1,0,1,0,0,0,0,0,0,0,0,1,0,1,0,0,1,0,1,0,0,0,1,0,1},
    {1,0,1,0,1,1,1,1,1,0,1,1,0,1,0,1,1,0,1,0,1,0,1,0,1},
    {1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
    {1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1},
    {1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1},
    {1,0,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1},
    {1,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1}, // Exit tile '2' is here!
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1} // 18th row
};


int main()
{
    // 1. Create the game window and initial setup
    sf::RenderWindow window(sf::VideoMode(800, 600), "Labyrinth Game - Phase 2");
    window.setFramerateLimit(60);

    // TILE_SIZE is defined in GameObject.h
    const float TILE_SIZE_FLOAT = static_cast<float>(TILE_SIZE);

    // --- Character Setup ---
    // Player starts with 10 HP
    Player player(1.5f, 1.5f, TILE_SIZE_FLOAT, 10);

    // Monsters start with 3 HP and check the map for a valid spawn point
    Monster monster1(20.5f, 15.5f, TILE_SIZE_FLOAT, fixedMap, 3);
    Monster monster2(10.5f, 8.5f, TILE_SIZE_FLOAT, fixedMap, 3);

    // --- Camera/View Setup ---
    const float VIEW_WIDTH = 20.f * TILE_SIZE_FLOAT;
    const float VIEW_HEIGHT = 15.f * TILE_SIZE_FLOAT;
    sf::View gameView(sf::Vector2f(0, 0), sf::Vector2f(VIEW_WIDTH, VIEW_HEIGHT));
    gameView.setCenter(player.getPosition());

    sf::Clock clock; // Clock object for measuring Delta Time

    // 2. The Game Loop
    while (window.isOpen())
    {
        // --- 2a. Handle Events ---
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // --- 2b. Update Game State ---
        sf::Time deltaTime = clock.restart();
        float dt = deltaTime.asSeconds();

        // 1. Update Player and Monsters
        player.update(dt, fixedMap);
        monster1.update(dt, fixedMap);
        monster2.update(dt, fixedMap);

        // 2. Update Camera
        gameView.setCenter(player.getPosition());

        // 3. Check for Player-Monster Collision (Player takes damage)
        sf::FloatRect playerBounds = player.getBounds();

        // Check collision only if the player is alive
        if (player.isAlive())
        {
            // Check against Monster 1
            if (monster1.isAlive() && playerBounds.intersects(monster1.getBounds()))
            {
                player.takeDamage(1);
            }

            // Check against Monster 2
            if (monster2.isAlive() && playerBounds.intersects(monster2.getBounds()))
            {
                player.takeDamage(1);
            }
        }

        // 4. Check for Win Condition (The Exit Tile '2')
        int playerTileX = static_cast<int>(player.getPosition().x / TILE_SIZE_FLOAT);
        int playerTileY = static_cast<int>(player.getPosition().y / TILE_SIZE_FLOAT);

        if (player.isAlive() && playerTileY >= 0 && playerTileY < MAP_HEIGHT && playerTileX >= 0 && playerTileX < MAP_WIDTH)
        {
            if (fixedMap[playerTileY][playerTileX] == 2)
            {
                std::cout << "--- YOU FOUND THE EXIT! YOU WIN! ---" << std::endl;
                window.close();
            }
        }

        // 5. Check for Game Over (If Player is defeated)
        if (!player.isAlive())
        {
            std::cout << "--- GAME OVER! ---" << std::endl;
            window.close();
        }

        // --- 2c. Render (Draw everything) ---
        window.setView(gameView); // Apply the camera view
        window.clear(sf::Color(20, 20, 20)); // Clear to dark floor color

        // Setup shape for drawing tiles
        sf::RectangleShape tile(sf::Vector2f(TILE_SIZE_FLOAT, TILE_SIZE_FLOAT));

        // Draw Map
        for (int y = 0; y < MAP_HEIGHT; ++y)
        {
            for (int x = 0; x < MAP_WIDTH; ++x)
            {
                if (fixedMap[y][x] == 1) // Wall
                {
                    tile.setFillColor(sf::Color(100, 100, 100)); // Grey wall
                }
                else if (fixedMap[y][x] == 2) // Exit
                {
                    tile.setFillColor(sf::Color::Green); // Bright Green Exit!
                }

                // Draw walls and exits only
                if (fixedMap[y][x] == 1 || fixedMap[y][x] == 2)
                {
                    tile.setPosition(static_cast<float>(x * TILE_SIZE), static_cast<float>(y * TILE_SIZE));
                    window.draw(tile);
                }
            }
        }

        // Draw the monsters (only if alive)
        if (monster1.isAlive()) monster1.draw(window);
        if (monster2.isAlive()) monster2.draw(window);

        // Draw the player (only if alive)
        if (player.isAlive()) player.draw(window);

        window.display();
    }

    return 0;
}