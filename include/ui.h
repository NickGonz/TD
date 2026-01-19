#ifndef UI_H
#define UI_H

#include "raylib.h"
#include "world.h"
#include "tower.h"
#include "player.h"

// Hotbar data structure
struct Hotbar {
    int selectedSlot;           // Which slot is currently selected (0-8)
    TowerType slots[9];         // What tower type is in each slot
};

// Function declarations
void InitHotbar(Hotbar* hotbar);
void UpdateHotbar(Hotbar* hotbar);
void DrawHotbar(const Hotbar* hotbar);
void DrawPlacementCursor(const World* world, const Hotbar* hotbar, const Camera2D* camera, const Player* player);
void UpdatePlacement(World* world, const Hotbar* hotbar, const Camera2D* camera, const Player* player);

#endif // UI_H
