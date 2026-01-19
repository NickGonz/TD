#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"

// Forward declaration
struct World;

// Player data structure
struct Player {
    Vector2 position;      // X and Y position in the world
    float speed;           // How fast the player moves (pixels per second)
    float radius;          // Size of the player (for drawing a circle)
    int wood;              // Collected wood
    int stone;             // Collected stone
    float swingTimer;      // Time remaining in swing animation
    float swingOffset;     // Swing offset from facing direction
    float facingAngle;     // Angle player is facing (toward mouse)
};

// Function declarations
void InitPlayer(Player* player);
void UpdatePlayer(Player* player, const World* world, const Camera2D* camera, float deltaTime);
void DrawPlayer(const Player* player);
void PlayerSwing(Player* player);
Vector2 GetToolTipPosition(const Player* player);

#endif // PLAYER_H
