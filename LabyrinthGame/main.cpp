#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>        // For std::hypot
#include <string>       // For std::to_string
#include "Player.h"     
#include "Monster.h"    
#include "Utility.h"    

// TILE_SIZE is defined in Utility.h
const float TILE_SIZE_FLOAT = static_cast<float>(TILE_SIZE);

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

    // --- Character Setup ---
    Player player(1.5f, 1.5f, TILE_SIZE_FLOAT, 10); // Player starts with 10 HP
    Monster monster1(20.5f, 15.5f, TILE_SIZE_FLOAT, fixedMap, 3);
    Monster monster2(10.5f, 8.5f, TILE_SIZE_FLOAT, fixedMap, 3);

    // --- Camera/View Setup ---
    const float VIEW_WIDTH = 20.f * TILE_SIZE_FLOAT;
    const float VIEW_HEIGHT = 15.f * TILE_SIZE_FLOAT;
    sf::View gameView(sf::Vector2f(0, 0), sf::Vector2f(VIEW_WIDTH, VIEW_HEIGHT));
    gameView.setCenter(player.getPosition());

    sf::Clock clock;

    // --- Font and Text Setup (HUD) ---
    sf::Font font;
    // IMPORTANT: You must ensure 'Roboto.ttf' (or any other font) is in your x64/Debug folder!
    if (!font.loadFromFile("Roboto.ttf"))
    {
        std::cerr << "Error loading font file! Check x64/Debug folder." << std::endl;
        return -1;
    }

    sf::Text healthText;
    healthText.setFont(font);
    healthText.setCharacterSize(24);
    healthText.setFillColor(sf::Color::White);

    sf::Text statusText;
    statusText.setFont(font);
    statusText.setCharacterSize(60);
    statusText.setFillColor(sf::Color::Red);
    statusText.setStyle(sf::Text::Bold);

    bool gameIsOver = false;

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

        // Stop updates if the game is over
        if (gameIsOver)
        {
            // Wait for 3 seconds after game over message appears, then close
            if (clock.getElapsedTime().asSeconds() > 3.0f)
            {
                window.close();
            }
            continue; // Skip the rest of the update loop
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

        // 3. Check for Player-Monster Interaction and Combat
        sf::FloatRect playerBounds = player.getBounds();

        // Check if the player has triggered an attack this frame (Spacebar pressed and cooldown is up)
        bool playerAttacking = player.getAttackTimer().getElapsedTime().asSeconds() < dt;

        // Helper lambda function to handle interaction for any monster
        auto handleInteraction = [&](Monster& monster)
            {
                if (monster.isAlive())
                {
                    // A. Check for player-touching-monster collision (Player takes damage)
                    if (player.isAlive() && playerBounds.intersects(monster.getBounds()))
                    {
                        player.takeDamage(1);
                    }

                    // B. Check for Player Attack (Monster takes damage)
                    if (playerAttacking)
                    {
                        // Calculate distance from player to monster center
                        float distance = std::hypot(player.getPosition().x - monster.getPosition().x,
                            player.getPosition().y - monster.getPosition().y);

                        if (distance <= player.getAttackRange())
                        {
                            monster.takeDamage(player.getAttackDamage());
                        }
                    }
                }
            };

        handleInteraction(monster1);
        handleInteraction(monster2);


        // 4. Check for Win/Lose Conditions
        int playerTileX = static_cast<int>(player.getPosition().x / TILE_SIZE_FLOAT);
        int playerTileY = static_cast<int>(player.getPosition().y / TILE_SIZE_FLOAT);

        if (!player.isAlive())
        {
            statusText.setString("GAME OVER");
            gameIsOver = true;
            clock.restart();
        }
        else if (playerTileY >= 0 && playerTileY < MAP_HEIGHT && playerTileX >= 0 && playerTileX < MAP_WIDTH)
        {
            if (fixedMap[playerTileY][playerTileX] == 2)
            {
                statusText.setString("YOU WON!");
                statusText.setFillColor(sf::Color::Green);
                gameIsOver = true;
                clock.restart();
            }
        }

        // 5. Update Health Display Text
        healthText.setString("Health: " + std::to_string(player.getHealth()));


        // --- 2c. Render (Draw everything) ---

        // 1. Draw Game World (Relative to Camera)
        window.setView(gameView); // Apply the camera view
        window.clear(sf::Color(20, 20, 20));

        // ... (Map drawing code remains here) ...
        sf::RectangleShape tile(sf::Vector2f(TILE_SIZE_FLOAT, TILE_SIZE_FLOAT));
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

        // 2. Draw HUD (Fixed to Screen)
        window.setView(window.getDefaultView()); // Switch to fixed screen view

        // Position Health Text in the top-left corner
        healthText.setPosition(10.f, 10.f);
        window.draw(healthText);

        // Display the status message in the center if game is over
        if (gameIsOver)
        {
            // Center the message on the screen (800x600 window)
            sf::FloatRect textRect = statusText.getLocalBounds();
            statusText.setOrigin(textRect.left + textRect.width / 2.0f,
                textRect.top + textRect.height / 2.0f);
            statusText.setPosition(800.f / 2.0f, 600.f / 2.0f);
            window.draw(statusText);
        }

        window.display();
    }

    return 0;
}