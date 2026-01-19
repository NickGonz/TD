#include "ui.h"
#include "world.h"
#include <math.h>

void InitHotbar(Hotbar* hotbar) {
    hotbar->selectedSlot = -1;  // Start with nothing selected

    // Set up what's in each slot
    hotbar->slots[0] = TOWER_ARCHER;
    hotbar->slots[1] = TOWER_CANNON;
    hotbar->slots[2] = TOWER_MAGE;
    // Rest are empty
    for (int i = 3; i < 9; i++) {
        hotbar->slots[i] = TOWER_NONE;
    }
}

static void ToggleSlot(Hotbar* hotbar, int slot) {
    // If already selected, deselect. Otherwise select it.
    if (hotbar->selectedSlot == slot) {
        hotbar->selectedSlot = -1;
    } else {
        hotbar->selectedSlot = slot;
    }
}

void UpdateHotbar(Hotbar* hotbar) {
    // Escape to deselect
    if (IsKeyPressed(KEY_ESCAPE)) {
        hotbar->selectedSlot = -1;
    }

    // Check for number key presses (1-9) - toggle selection
    if (IsKeyPressed(KEY_ONE))   ToggleSlot(hotbar, 0);
    if (IsKeyPressed(KEY_TWO))   ToggleSlot(hotbar, 1);
    if (IsKeyPressed(KEY_THREE)) ToggleSlot(hotbar, 2);
    if (IsKeyPressed(KEY_FOUR))  ToggleSlot(hotbar, 3);
    if (IsKeyPressed(KEY_FIVE))  ToggleSlot(hotbar, 4);
    if (IsKeyPressed(KEY_SIX))   ToggleSlot(hotbar, 5);
    if (IsKeyPressed(KEY_SEVEN)) ToggleSlot(hotbar, 6);
    if (IsKeyPressed(KEY_EIGHT)) ToggleSlot(hotbar, 7);
    if (IsKeyPressed(KEY_NINE))  ToggleSlot(hotbar, 8);
}

void DrawHotbar(const Hotbar* hotbar) {
    int slotSize = 50;   // Each slot is 50x50 pixels
    int padding = 5;     // Space between slots
    int totalWidth = 9 * slotSize + 8 * padding;  // 9 slots + 8 gaps
    int startX = (GetScreenWidth() - totalWidth) / 2;  // Center horizontally
    int startY = GetScreenHeight() - slotSize - 10;    // 10px from bottom

    for (int i = 0; i < 9; i++) {
        int x = startX + i * (slotSize + padding);

        // Draw slot background
        DrawRectangle(x, startY, slotSize, slotSize, DARKGRAY);

        // Draw tower icon if slot has one
        int iconPadding = 8;
        int iconSize = slotSize - iconPadding * 2;
        if (hotbar->slots[i] == TOWER_ARCHER) {
            DrawRectangle(x + iconPadding, startY + iconPadding, iconSize, iconSize, GREEN);
        } else if (hotbar->slots[i] == TOWER_CANNON) {
            DrawRectangle(x + iconPadding, startY + iconPadding, iconSize, iconSize, RED);
        } else if (hotbar->slots[i] == TOWER_MAGE) {
            DrawRectangle(x + iconPadding, startY + iconPadding, iconSize, iconSize, PURPLE);
        }

        // Draw border - yellow if selected, black otherwise
        Color borderColor = (i == hotbar->selectedSlot) ? YELLOW : BLACK;
        DrawRectangleLinesEx((Rectangle){(float)x, (float)startY, (float)slotSize, (float)slotSize}, 2, borderColor);
    }
}

static bool CheckPlayerCollision(Vector2 position, float radius, const Player* player) {
    float dx = position.x - player->position.x;
    float dy = position.y - player->position.y;
    float dist = sqrtf(dx * dx + dy * dy);
    return dist < (radius + player->radius);
}

void DrawPlacementCursor(const World* world, const Hotbar* hotbar, const Camera2D* camera, const Player* player) {
    // Only show cursor if a slot is selected
    if (hotbar->selectedSlot < 0) {
        return;  // Nothing selected
    }

    TowerType selectedType = hotbar->slots[hotbar->selectedSlot];
    if (selectedType == TOWER_NONE) {
        return;  // Selected slot is empty
    }

    // Get mouse position in world coordinates
    Vector2 mouseScreen = GetMousePosition();
    Vector2 mouseWorld = GetScreenToWorld2D(mouseScreen, *camera);

    float radius = 16.0f;  // Same as tower radius

    // Check if we can place here (no collision with towers, resources, or player)
    bool canPlace = IsInWorldBounds(world, mouseWorld, radius) &&
                    !CheckAnyCollision(world, mouseWorld, radius) &&
                    !CheckPlayerCollision(mouseWorld, radius, player);

    // Choose color based on validity (semi-transparent)
    Color cursorColor;
    if (canPlace) {
        cursorColor = Color{0, 255, 0, 100};  // Green, can place
    } else {
        cursorColor = Color{255, 0, 0, 100};  // Red, can't place
    }

    // Draw the preview circle at mouse position (in world space)
    DrawCircleV(mouseWorld, radius, cursorColor);
}

void UpdatePlacement(World* world, const Hotbar* hotbar, const Camera2D* camera, const Player* player) {
    // Only place if a slot is selected
    if (hotbar->selectedSlot < 0) {
        return;
    }

    TowerType selectedType = hotbar->slots[hotbar->selectedSlot];
    if (selectedType == TOWER_NONE) {
        return;
    }

    // Check for left mouse click
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mouseScreen = GetMousePosition();
        Vector2 mouseWorld = GetScreenToWorld2D(mouseScreen, *camera);

        // Don't place on top of the player
        float towerRadius = 16.0f;
        if (CheckPlayerCollision(mouseWorld, towerRadius, player)) {
            return;
        }

        // Try to place the tower at mouse position
        PlaceTower(world, mouseWorld, selectedType);
    }
}
