#include "player.h"
#include <math.h>

void InitPlayer(Player* player) {
    player->position.x = 100.0f;
    player->position.y = 200.0f;
    player->speed = 200.0f;
    player->radius = 15.0f;
   
}

void UpdatePlayer(Player* player, float deltaTime) {
    Vector2 direction = {0.0f, 0.0f};

    if (IsKeyDown(KEY_W)) direction.y -= 1.0f;
    if (IsKeyDown(KEY_S)) direction.y += 1.0f;
    if (IsKeyDown(KEY_A)) direction.x -= 1.0f;
    if (IsKeyDown(KEY_D)) direction.x += 1.0f;

    float length = sqrtf(direction.x * direction.x + direction.y * direction.y);
    if (length > 0.0f) {
        direction.x /= length;
        direction.y /= length;

        player->position.x += direction.x * player->speed * deltaTime;
        player->position.y += direction.y * player->speed * deltaTime;
    }
}

void DrawPlayer(const Player* player) {
    DrawCircleV(player->position, player->radius, BEIGE);
}
