#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"

// Player data structure
struct Player {
    Vector2 position;      // X and Y position in the world
    float speed;           // How fast the player moves (pixels per second)
    float radius;          // Size of the player (for drawing a circle)
};

// Function declarations - YOU will implement these!
void InitPlayer(Player* player);                    // Set up player starting values
void UpdatePlayer(Player* player, float deltaTime); // Handle movement input
void DrawPlayer(const Player* player);              // Draw the player on screen

#endif // PLAYER_H
