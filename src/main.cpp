#include "raylib.h"
#include "game.h"
#include "player.h"
#include "world.h"

int main() {
    // Window setup
    InitWindow(800, 600, "Tower Defense");
    SetTargetFPS(60);

    // Initialize game state
    GameState game;
    InitGame(&game);

    // Initialize world
    World world;
    InitWorld(&world);

    // Initialize player
    Player player;
    InitPlayer(&player);

    // Main game loop
    while (!WindowShouldClose()) {
        // UPDATE: Game logic
        UpdateGame(&game, GetFrameTime());
        UpdatePlayer(&player, GetFrameTime());

        // DRAW: Rendering
        BeginDrawing();

        // Change background color based on phase
        Color bgColor = (game.currentPhase == PHASE_DAY) ? RAYWHITE : DARKGRAY;
        ClearBackground(bgColor);

        // Draw the world (grid and walls)
        DrawWorld(&world);

        // Draw the player
        DrawPlayer(&player);

        // Draw the game UI (on top)
        DrawGameUI(&game);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}