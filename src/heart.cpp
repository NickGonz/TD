#include "heart.h"
#include "world.h"  // For TILE_SIZE

void InitHeart(Heart* heart, int gridX, int gridY) {
    heart->gridX = gridX;
    heart->gridY = gridY;
    heart->maxHealth = 100;
    heart->currentHealth = 100;
}

void DrawHeart(const Heart* heart) {
    int pixelX = heart->gridX * TILE_SIZE;
    int pixelY = heart->gridY * TILE_SIZE;

    // Draw the heart as a red/pink square for now
    // Later can be replaced with a sprite
    DrawRectangle(pixelX, pixelY, TILE_SIZE, TILE_SIZE, MAROON);

    // Draw a simple heart shape on top
    int centerX = pixelX + TILE_SIZE / 2;
    int centerY = pixelY + TILE_SIZE / 2;
    DrawCircle(centerX - 6, centerY - 4, 8, RED);
    DrawCircle(centerX + 6, centerY - 4, 8, RED);
    DrawTriangle(
        (Vector2){(float)centerX - 14, (float)centerY - 2},
        (Vector2){(float)centerX + 14, (float)centerY - 2},
        (Vector2){(float)centerX, (float)centerY + 12},
        RED
    );

    // Draw health bar above
    int barWidth = TILE_SIZE;
    int barHeight = 6;
    int barX = pixelX;
    int barY = pixelY - 10;

    float healthPercent = (float)heart->currentHealth / heart->maxHealth;

    // Background (gray)
    DrawRectangle(barX, barY, barWidth, barHeight, DARKGRAY);
    // Health (green to red based on health)
    Color healthColor = (healthPercent > 0.5f) ? GREEN : (healthPercent > 0.25f) ? YELLOW : RED;
    DrawRectangle(barX, barY, (int)(barWidth * healthPercent), barHeight, healthColor);
    // Border
    DrawRectangleLines(barX, barY, barWidth, barHeight, BLACK);
}

void DamageHeart(Heart* heart, int damage) {
    heart->currentHealth -= damage;
    if (heart->currentHealth < 0) {
        heart->currentHealth = 0;
    }
}

bool IsHeartAlive(const Heart* heart) {
    return heart->currentHealth > 0;
}
