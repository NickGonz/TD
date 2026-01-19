#ifndef TOWER_H
#define TOWER_H

#include "raylib.h"

enum TowerType {
    TOWER_NONE,
    TOWER_ARCHER,
    TOWER_CANNON,
    TOWER_MAGE
};

struct Tower {
    Vector2 position;  // World position (center of tower)
    float radius;      // Collision radius
    TowerType type;
};

#endif  // TOWER_H