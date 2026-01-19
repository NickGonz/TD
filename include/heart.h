#ifndef HEART_H
#define HEART_H

#include "raylib.h"

struct Heart {
    int gridX;          // Position on the grid
    int gridY;
    int maxHealth;
    int currentHealth;
};

void InitHeart(Heart* heart, int gridX, int gridY);
void DrawHeart(const Heart* heart);
void DamageHeart(Heart* heart, int damage);
bool IsHeartAlive(const Heart* heart);

#endif // HEART_H
