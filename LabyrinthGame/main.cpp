#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>        // For std::hypot
#include <string>       // For std::to_string
#include <list>         // For std::list
#include "Player.h"     
#include "Monster.h"    
#include "Utility.h"    
#include "DamageIndicator.h" 

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
    // SFML 2.x Syntax: Direct VideoMode constructor
    sf::RenderWindow window(sf::VideoMode(800, 600), "Labyrinth Game - Phase 2");
    window.setFramerateLimit(60);

    // --- Character Setup ---
    Player player(1.5f, 1.5f, TILE_SIZE_FLOAT, 10);
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
    // SFML 2.x Syntax: Simple loadFromFile
    if (!font.loadFromFile("Roboto.ttf"))
    {
        std::cerr << "Error loading font file! Check x64/Debug folder." << std::endl;
        return -1;
    }

    // SFML 2.x Syntax: Default constructor, then setup
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

    // --- Combat Feedback List ---
    std::list<DamageIndicator> damageIndicators;

    // 2. The Game Loop
    while (window.isOpen())
    {
        // --- 2a. Handle Events ---
        sf::Event event;
        while (window.pollEvent(event))
        {
            // SFML 2.x Syntax: Direct member access for event type
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Stop updates if the game is over
        if (gameIsOver)
        {
            if (clock.getElapsedTime().asSeconds() > 3.0f)
            {
                window.close();
            }
            continue;
        }

        // --- 2b. Update Game State ---
        sf::Time deltaTime = clock.restart();
        float dt = deltaTime.asSeconds();

        player.update(dt, fixedMap);
        monster1.update(dt, fixedMap);
        monster2.update(dt, fixedMap);

        gameView.setCenter(player.getPosition());

        // 3. Check for Player-Monster Interaction and Combat
        sf::FloatRect playerBounds = player.getBounds();

        // SFML 2.x Syntax: Check isKeyPressed without scoped enum
        bool playerAttacking = sf::Keyboard::isKeyPressed(sf::Keyboard::Space) &&
            player.getAttackTimer().getElapsedTime().asSeconds() < player.getAttackCooldown() + dt &&
            player.getAttackTimer().getElapsedTime().asSeconds() >= player.getAttackCooldown();


        auto handleInteraction = [&](Monster& monster)
            {
                if (monster.isAlive())
                {
                    // A. Player takes damage 
                    if (player.isAlive() && playerBounds.intersects(monster.getBounds()))
                    {
                        if (!player.isInvincible())
                        {
                            player.takeDamage(1);

                            damageIndicators.emplace_back(
                                player.getPosition(),
                                1,
                                true,
                                font
                            );
                        }
                    }

                    // B. Check for Player Attack 
                    if (playerAttacking)
                    {
                        float distance = std::hypot(player.getPosition().x - monster.getPosition().x,
                            player.getPosition().y - monster.getPosition().y);

                        if (distance <= player.getAttackRange())
                        {
                            if (!monster.isInvincible())
                            {
                                monster.takeDamage(player.getAttackDamage());

                                damageIndicators.emplace_back(
                                    monster.getPosition(),
                                    player.getAttackDamage(),
                                    false,
                                    font
                                );
                            }
                        }
                    }
                }
            };

        handleInteraction(monster1);
        handleInteraction(monster2);

        // --- Update and Clean Up Damage Indicators ---
        for (auto it = damageIndicators.begin(); it != damageIndicators.end(); )
        {
            if (it->update(dt))
            {
                it = damageIndicators.erase(it);
            }
            else
            {
                ++it;
            }
        }

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
        window.setView(gameView);
        window.clear(sf::Color(20, 20, 20));

        sf::RectangleShape tile(sf::Vector2f(TILE_SIZE_FLOAT, TILE_SIZE_FLOAT));
        for (int y = 0; y < MAP_HEIGHT; ++y)
        {
            for (int x = 0; x < MAP_WIDTH; ++x)
            {
                if (fixedMap[y][x] == 1)
                {
                    tile.setFillColor(sf::Color(100, 100, 100));
                }
                else if (fixedMap[y][x] == 2)
                {
                    tile.setFillColor(sf::Color::Green);
                }

                if (fixedMap[y][x] == 1 || fixedMap[y][x] == 2)
                {
                    tile.setPosition(static_cast<float>(x * TILE_SIZE), static_cast<float>(y * TILE_SIZE));
                    window.draw(tile);
                }
            }
        }

        if (monster1.isAlive()) monster1.draw(window);
        if (monster2.isAlive()) monster2.draw(window);

        if (player.isAlive()) player.draw(window);

        // Draw ALL Active Damage Indicators (Floats in game world)
        for (const auto& indicator : damageIndicators)
        {
            indicator.draw(window);
        }

        // 2. Draw HUD (Fixed to Screen)
        window.setView(window.getDefaultView());

        healthText.setPosition(10.f, 10.f);
        window.draw(healthText);

        if (gameIsOver)
        {
            sf::FloatRect textRect = statusText.getLocalBounds();
            // SFML 2.x Syntax: Direct member access
            statusText.setOrigin(textRect.left + textRect.width / 2.0f,
                textRect.top + textRect.height / 2.0f);
            statusText.setPosition(800.f / 2.0f, 600.f / 2.0f);
            window.draw(statusText);
        }

        window.display();
    }

    return 0;
}