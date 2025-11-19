#include <SFML/Graphics.hpp>
#include <iostream>

// Define constants for the game world
const int TILE_SIZE = 32;
const int MAP_WIDTH = 25; // 25 tiles * 32px/tile = 800px (Fits window)
const int MAP_HEIGHT = 18; // 18 tiles * 32px/tile = 576px (Fits window)

// Helper function to check if a map coordinate is a wall (1)
bool isWall(int mapX, int mapY, const int map[MAP_HEIGHT][MAP_WIDTH])
{
    // 1. Basic Bounds Check: Treat outside the map as a wall
    if (mapY < 0 || mapY >= MAP_HEIGHT || mapX < 0 || mapX >= MAP_WIDTH) {
        return true;
    }

    // 2. Check the array value: If it's 1, it's a wall.
    return map[mapY][mapX] == 1;
}

// The 2D array representing the map: 1 = Wall, 0 = Floor, 2 = Exit (to be used later)
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
    // 1. Create the game window (The screen)
    sf::RenderWindow window(sf::VideoMode(800, 600), "Labyrinth Game - Phase 1");
    window.setFramerateLimit(60); // Limit to 60 frames per second

    // --- Player Setup ---
    // Note: TILE_SIZE is defined as 32 at the top

    // Create the red square player object
    sf::RectangleShape player(sf::Vector2f(TILE_SIZE * 0.8f, TILE_SIZE * 0.8f));
    player.setFillColor(sf::Color::Red);
    // Set origin to center for easier positioning, although we are using top-left collision here
    player.setOrigin(player.getSize() / 2.f);

    // Starting position (1.5, 1.5 is the center of the open tile at fixedMap[1][1])
    player.setPosition(1.5f * TILE_SIZE, 1.5f * TILE_SIZE);

    // Movement variables
    float playerSpeed = 150.f; // The player will move at 150 pixels per second
    sf::Clock clock; // Clock object for measuring Delta Time

    // --- Camera/View Setup ---
    // Define the size of the visible area (e.g., 20 tiles wide x 15 tiles high)
    const float VIEW_WIDTH = 20.f * TILE_SIZE;
    const float VIEW_HEIGHT = 15.f * TILE_SIZE;

    // 1. Create the View object with the desired size
    sf::View gameView(sf::Vector2f(0, 0), sf::Vector2f(VIEW_WIDTH, VIEW_HEIGHT));

    // 2. Center the view initially on the player's starting position
    gameView.setCenter(player.getPosition());

    // 2. The Game Loop (runs until the window is closed)
    while (window.isOpen())
    {
        // --- 2a. Handle Events (Input) ---
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // --- 2b. Update Game State (Collision Check) ---

           // Measure the time since the last frame was rendered
        sf::Time deltaTime = clock.restart();
        float dt = deltaTime.asSeconds();

        sf::Vector2f movement(0.f, 0.f);

        // --- Calculate Intentional Movement (Keep your WASD input checks here) ---
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            movement.y -= playerSpeed * dt;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            movement.y += playerSpeed * dt;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            movement.x -= playerSpeed * dt;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            movement.x += playerSpeed * dt;

        // --- Collision Check and Movement Application ---

        // --- 1. Check X-axis movement ---
        sf::Vector2f nextPos_X = player.getPosition() + sf::Vector2f(movement.x, 0.f);

        // Calculate the map coordinates for the player's center after X movement
        int checkTileX_X = static_cast<int>(nextPos_X.x / TILE_SIZE);
        int checkTileY_X = static_cast<int>(nextPos_X.y / TILE_SIZE); // Y position remains the same

        // Check if the intended X position lands the player's center in a wall tile
        if (!isWall(checkTileX_X, checkTileY_X, fixedMap)) {
            // If the tile is NOT a wall, it's safe to move horizontally
            player.move(movement.x, 0.f);
        }

        // --- 2. Check Y-axis movement ---
        sf::Vector2f nextPos_Y = player.getPosition() + sf::Vector2f(0.f, movement.y);

        // Calculate the map coordinates for the player's center after Y movement
        int checkTileX_Y = static_cast<int>(nextPos_Y.x / TILE_SIZE); // X position remains the same
        int checkTileY_Y = static_cast<int>(nextPos_Y.y / TILE_SIZE);

        // Check if the intended Y position lands the player's center in a wall tile
        if (!isWall(checkTileX_Y, checkTileY_Y, fixedMap)) {
            // If the tile is NOT a wall, it's safe to move vertically
            player.move(0.f, movement.y);
        }
        // --- Camera Update (New Step) ---
        // Smoothly move the camera's center to follow the player's position
        gameView.setCenter(player.getPosition());

        // 3. CHECK FOR WIN CONDITION (Add it here, immediately after movement)

        // Get the current tile coordinates of the player's center
        int playerTileX = static_cast<int>(player.getPosition().x / TILE_SIZE);
        int playerTileY = static_cast<int>(player.getPosition().y / TILE_SIZE);

        // Ensure coordinates are within bounds before checking the map array
        if (playerTileY >= 0 && playerTileY < MAP_HEIGHT && playerTileX >= 0 && playerTileX < MAP_WIDTH)
        {
            // Check if the current tile the player is standing on is the Exit tile (2)
            if (fixedMap[playerTileY][playerTileX] == 2)
            {
                std::cout << "--- YOU FOUND THE EXIT! YOU WIN! ---" << std::endl;
                window.close(); // End the game
            }
        }
        // --- APPLY VIEW (New Step) ---
        window.setView(gameView);
        // --- 2c. Render (Draw everything) ---
        window.clear(sf::Color(20, 20, 20)); // Clear the window to a dark color

        // Setup a reusable shape for drawing tiles
        sf::RectangleShape tile(sf::Vector2f(TILE_SIZE, TILE_SIZE));

        // Nested loop to iterate through every cell in the map array
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

                // Only draw if it's a wall or an exit (we rely on window.clear for the floor/dark background)
                if (fixedMap[y][x] == 1 || fixedMap[y][x] == 2)
                {
                    tile.setPosition(static_cast<float>(x * TILE_SIZE), static_cast<float>(y * TILE_SIZE));
                    window.draw(tile);
                }
            }
        }

        // Draw the player on top of the map
        window.draw(player);

        window.display(); // Display the updated content of the window
    }

    return 0;
}