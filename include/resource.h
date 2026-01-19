#ifndef RESOURCE_H
#define RESOURCE_H

#include "raylib.h"

enum ResourceType {
    RESOURCE_TREE,
    RESOURCE_ROCK
};

struct Resource {
    Vector2 position;
    float radius;
    ResourceType type;
    int health;      // Hits remaining before depleted
    int maxHealth;   // For respawning
    float shakeTimer;  // Time remaining for shake animation
};

#define MAX_RESOURCES 500

#endif // RESOURCE_H
