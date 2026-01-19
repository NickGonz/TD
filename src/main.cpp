#include "raylib.h"
#include "game.h"
#include "player.h"
#include "world.h"
#include "ui.h"
#include "heart.h"

int main() {

    InitWindow(1240, 720, "Tower Defense");
    SetTargetFPS(60);

    GameState game;
    InitGame(&game);

    World world;
    InitWorld(&world);
    LoadWorldAssets(&world);
    SpawnResources(&world, 200);  // Spawn trees and rocks

    Player player;
    InitPlayer(&player);

    Hotbar hotbar;
    InitHotbar(&hotbar);

    // Place heart in center of the map
    Heart heart;
    InitHeart(&heart, WORLD_WIDTH / 2, WORLD_HEIGHT / 2);

    // Set up camera
    Camera2D camera = { 0 };
    camera.offset = (Vector2){ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.55f;  // Start zoomed out a bit to see more of the world

    while (!WindowShouldClose()) {

        float dt = GetFrameTime();
        UpdateGame(&game, dt);
        UpdatePlayer(&player, &world, &camera, dt);
        UpdateResources(&world, dt);
        UpdateHotbar(&hotbar);
        UpdatePlacement(&world, &hotbar, &camera, &player);

        // Left-click to gather resources (only when nothing selected in hotbar)
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && hotbar.selectedSlot < 0) {
            PlayerSwing(&player);
            // Use tool tip position for gathering
            Vector2 toolTip = GetToolTipPosition(&player);
            float gatherReach = 40.0f;  // Radius around tool tip
            int gathered = GatherResource(&world, toolTip, gatherReach);
            if (gathered == 1) {
                player.wood++;
            } else if (gathered == 2) {
                player.stone++;
            }
        }

        // Camera zoom with scroll wheel
        float wheel = GetMouseWheelMove();
        if (wheel != 0) {
            camera.zoom += wheel * 0.1f;
            if (camera.zoom < 0.25f) camera.zoom = 0.25f;  // Min zoom (zoomed out)
            if (camera.zoom > 2.0f) camera.zoom = 2.0f;    // Max zoom (zoomed in)
        }

        // Update camera to follow player
        camera.target = player.position;

        BeginDrawing();

        Color bgColor = (game.currentPhase == PHASE_DAY) ? RAYWHITE : DARKGRAY;
        ClearBackground(bgColor);

        // Begin camera mode - world space drawing
        BeginMode2D(camera);

        DrawWorld(&world);
        DrawResources(&world);
        DrawTowers(&world);
        DrawHeart(&heart);
        DrawPlayer(&player);
        DrawPlacementCursor(&world, &hotbar, &camera, &player);

        EndMode2D();
        // End camera mode - UI drawing (screen space)

        DrawHotbar(&hotbar);
        DrawGameUI(&game);

        // Draw resource counts (above hotbar)
        int hotbarY = GetScreenHeight() - 50 - 10;  // Match hotbar position
        DrawText(TextFormat("Wood: %d  Stone: %d", player.wood, player.stone),
                 GetScreenWidth() / 2 - 80, hotbarY - 30, 20, WHITE);

        EndDrawing();
    }

    UnloadWorldAssets(&world);
    CloseWindow();
    return 0;
}