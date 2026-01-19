#include "player.h"
#include "world.h"
#include <math.h>

void InitPlayer(Player* player) {
    // Start player near center of world
    player->position.x = (WORLD_WIDTH * TILE_SIZE) / 2.0f;
    player->position.y = (WORLD_HEIGHT * TILE_SIZE) / 2.0f + 100.0f;
    player->speed = 200.0f;
    player->radius = 15.0f;
    player->wood = 0;
    player->stone = 0;
    player->swingTimer = 0.0f;
    player->swingOffset = 0.0f;
    player->facingAngle = 0.0f;
}

void UpdatePlayer(Player* player, const World* world, const Camera2D* camera, float deltaTime) {
    Vector2 direction = {0.0f, 0.0f};

    if (IsKeyDown(KEY_W)) direction.y -= 1.0f;
    if (IsKeyDown(KEY_S)) direction.y += 1.0f;
    if (IsKeyDown(KEY_A)) direction.x -= 1.0f;
    if (IsKeyDown(KEY_D)) direction.x += 1.0f;

    float length = sqrtf(direction.x * direction.x + direction.y * direction.y);
    if (length > 0.0f) {
        direction.x /= length;
        direction.y /= length;

        // Try moving on X axis
        Vector2 newPosX = {
            player->position.x + direction.x * player->speed * deltaTime,
            player->position.y
        };
        if (!CheckAnyCollision(world, newPosX, player->radius)) {
            player->position.x = newPosX.x;
        }

        // Try moving on Y axis
        Vector2 newPosY = {
            player->position.x,
            player->position.y + direction.y * player->speed * deltaTime
        };
        if (!CheckAnyCollision(world, newPosY, player->radius)) {
            player->position.y = newPosY.y;
        }
    }

    // Face toward mouse cursor
    Vector2 mouseScreen = GetMousePosition();
    Vector2 mouseWorld = GetScreenToWorld2D(mouseScreen, *camera);
    float dx = mouseWorld.x - player->position.x;
    float dy = mouseWorld.y - player->position.y;
    player->facingAngle = atan2f(dy, dx) * (180.0f / 3.14159f);

    // Update swing animation
    if (player->swingTimer > 0.0f) {
        player->swingTimer -= deltaTime;
        // Swing arc: start at -45 offset, swing to +45 offset
        float swingDuration = 0.2f;
        float progress = 1.0f - (player->swingTimer / swingDuration);
        player->swingOffset = -45.0f + (progress * 90.0f);
        if (player->swingTimer <= 0.0f) {
            player->swingTimer = 0.0f;
            player->swingOffset = 0.0f;
        }
    }
}

void PlayerSwing(Player* player) {
    if (player->swingTimer <= 0.0f) {
        player->swingTimer = 0.2f;  // 0.2 second swing
        player->swingOffset = -45.0f;  // Start offset from facing
    }
}

Vector2 GetToolTipPosition(const Player* player) {
    float toolReach = 35.0f;  // Distance from player center to tool tip

    // Tool points in facing direction, offset by swing
    float angle = (player->facingAngle + player->swingOffset) * (3.14159f / 180.0f);

    return (Vector2){
        player->position.x + cosf(angle) * toolReach,
        player->position.y + sinf(angle) * toolReach
    };
}

void DrawPlayer(const Player* player) {
    // Tool angle: facing direction + swing offset
    float angle = (player->facingAngle + player->swingOffset) * (3.14159f / 180.0f);

    // Pickaxe settings
    float handleLength = 20.0f;
    float handleThickness = 4.0f;
    float headWidth = 14.0f;
    float headThickness = 5.0f;

    // Handle starts at edge of player
    Vector2 handleStart = {
        player->position.x + cosf(angle) * player->radius,
        player->position.y + sinf(angle) * player->radius
    };
    Vector2 handleEnd = {
        player->position.x + cosf(angle) * (player->radius + handleLength),
        player->position.y + sinf(angle) * (player->radius + handleLength)
    };

    // Pickaxe head is perpendicular to handle at the end
    float perpAngle = angle + 3.14159f / 2.0f;
    Vector2 headLeft = {
        handleEnd.x - cosf(perpAngle) * headWidth * 0.5f,
        handleEnd.y - sinf(perpAngle) * headWidth * 0.5f
    };
    Vector2 headRight = {
        handleEnd.x + cosf(perpAngle) * headWidth * 0.5f,
        handleEnd.y + sinf(perpAngle) * headWidth * 0.5f
    };

    // Draw handle (brown wood)
    DrawLineEx(handleStart, handleEnd, handleThickness, BROWN);

    // Draw pickaxe head (gray metal)
    DrawLineEx(headLeft, headRight, headThickness, DARKGRAY);

    // Draw player body on top
    DrawCircleV(player->position, player->radius, BEIGE);
}
