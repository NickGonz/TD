#include "world.h"
#include <stdlib.h>
#include <math.h>

void InitWorld(World* world) {
    world->width = WORLD_WIDTH;
    world->height = WORLD_HEIGHT;
    world->tileSize = TILE_SIZE;

    // All tiles are empty (no walls)
    for (int y = 0; y < WORLD_HEIGHT; y++) {
        for (int x = 0; x < WORLD_WIDTH; x++) {
            world->tiles[y][x] = TILE_EMPTY;
        }
    }

    // Initialize visual tiles - all solid dirt (column 0)
    for (int y = 0; y < WORLD_HEIGHT * 2; y++) {
        for (int x = 0; x < WORLD_WIDTH * 2; x++) {
            world->visualTiles[y][x] = 0;  // Solid dirt base
        }
    }

    // Initialize decoration layer with sparse random decorations
    // Only use small grass/debris decorations, avoid the vertical lines
    // Row 1 columns 0-3 seem to have the small scattered debris
    for (int y = 0; y < WORLD_HEIGHT * 2; y++) {
        for (int x = 0; x < WORLD_WIDTH * 2; x++) {
            int roll = rand() % 100;
            if (roll < 12) {
                // 12% chance of decoration - only use columns 0-3 from row 1
                world->decorationTiles[y][x] = rand() % 4;
            } else {
                world->decorationTiles[y][x] = -1;  // No decoration
            }
        }
    }

    // Initialize tower storage
    world->towerCount = 0;

    // Initialize resource storage
    world->resourceCount = 0;
}

void LoadWorldAssets(World* world) {
    world->tileset = LoadTexture("assets/wasteland bg.png");
}

void UnloadWorldAssets(World* world) {
    UnloadTexture(world->tileset);
}

void DrawWorld(const World* world) {
    // Draw visual tiles (16px sprites)
    for (int y = 0; y < WORLD_HEIGHT * 2; y++) {
        for (int x = 0; x < WORLD_WIDTH * 2; x++) {
            int pixelX = x * SPRITE_SIZE;
            int pixelY = y * SPRITE_SIZE;

            // Get sprite index for this visual tile
            int spriteIndex = world->visualTiles[y][x];

            // Source rectangle from spritesheet (16x16 tiles in top row)
            Rectangle src = {
                (float)(spriteIndex * SPRITE_SIZE),  // x position in sheet
                0,                                    // y position (top row)
                SPRITE_SIZE,
                SPRITE_SIZE
            };

            // Destination rectangle on screen
            Rectangle dest = {
                (float)pixelX,
                (float)pixelY,
                SPRITE_SIZE,
                SPRITE_SIZE
            };

            DrawTexturePro(world->tileset, src, dest, (Vector2){0, 0}, 0, WHITE);

            // Draw decoration if present
            int decoIndex = world->decorationTiles[y][x];
            if (decoIndex >= 0) {
                int decoCol = decoIndex % 8;           // Column in spritesheet (0-7)
                int decoRow = (decoIndex < 8) ? 1 : 2; // Row 1 or Row 2
                Rectangle decoSrc = {
                    (float)(decoCol * SPRITE_SIZE),
                    (float)(decoRow * SPRITE_SIZE),
                    SPRITE_SIZE,
                    SPRITE_SIZE
                };
                DrawTexturePro(world->tileset, decoSrc, dest, (Vector2){0, 0}, 0, WHITE);
            }
        }
    }
}

    bool IsTileWalkable(const World* world, int gridX, int gridY) {
    if (gridX < 0 || gridX >= world->width) return false;
    if (gridY < 0 || gridY >= world->height) return false;

    if (world->tiles[gridY][gridX] == TILE_WALL) return false;

    return true;
}

void WorldToGrid(float worldX, float worldY, int* gridX, int* gridY) {
    *gridX = (int)(worldX / TILE_SIZE);
    *gridY = (int)(worldY / TILE_SIZE);
}

bool CheckTowerCollision(const World* world, Vector2 position, float radius) {
    // Check if this position overlaps with any existing tower
    for (int i = 0; i < world->towerCount; i++) {
        const Tower* tower = &world->towers[i];
        float dx = position.x - tower->position.x;
        float dy = position.y - tower->position.y;
        float distance = sqrtf(dx * dx + dy * dy);
        float minDist = radius + tower->radius;

        if (distance < minDist) {
            return true;  // Collision!
        }
    }
    return false;
}

bool CheckResourceCollision(const World* world, Vector2 position, float radius) {
    // Check if this position overlaps with any resource
    for (int i = 0; i < world->resourceCount; i++) {
        const Resource* res = &world->resources[i];
        float dx = position.x - res->position.x;
        float dy = position.y - res->position.y;
        float distance = sqrtf(dx * dx + dy * dy);
        float minDist = radius + res->radius;

        if (distance < minDist) {
            return true;
        }
    }
    return false;
}

bool CheckAnyCollision(const World* world, Vector2 position, float radius) {
    return CheckTowerCollision(world, position, radius) ||
           CheckResourceCollision(world, position, radius);
}

bool IsInWorldBounds(const World* world, Vector2 position, float radius) {
    float worldPixelWidth = world->width * TILE_SIZE;
    float worldPixelHeight = world->height * TILE_SIZE;

    return position.x - radius >= 0 &&
           position.x + radius <= worldPixelWidth &&
           position.y - radius >= 0 &&
           position.y + radius <= worldPixelHeight;
}

bool PlaceTower(World* world, Vector2 position, TowerType type) {
    float radius = 16.0f;  // Tower collision radius

    // Check bounds
    if (!IsInWorldBounds(world, position, radius)) return false;

    // Check collision with existing towers and resources
    if (CheckAnyCollision(world, position, radius)) return false;

    // Check max towers
    if (world->towerCount >= MAX_TOWERS) return false;

    // Add the tower
    Tower* tower = &world->towers[world->towerCount];
    tower->position = position;
    tower->radius = radius;
    tower->type = type;
    world->towerCount++;

    return true;
}

void DrawTowers(const World* world) {
    for (int i = 0; i < world->towerCount; i++) {
        const Tower* tower = &world->towers[i];

        // Choose color based on tower type
        Color color;
        if (tower->type == TOWER_ARCHER) {
            color = GREEN;
        } else if (tower->type == TOWER_CANNON) {
            color = RED;
        } else if (tower->type == TOWER_MAGE) {
            color = PURPLE;
        } else {
            color = GRAY;
        }

        // Draw tower as a circle at its position
        DrawCircleV(tower->position, tower->radius, color);
    }
}

static bool TrySpawnResource(World* world, Vector2 position, ResourceType type) {
    float radius = (type == RESOURCE_TREE) ? 40.0f : 20.0f;

    if (!IsInWorldBounds(world, position, radius)) {
        return false;
    }

    if (CheckResourceCollision(world, position, radius)) {
        return false;
    }

    if (world->resourceCount >= MAX_RESOURCES) {
        return false;
    }

    Resource* res = &world->resources[world->resourceCount];
    res->position = position;
    res->radius = radius;
    res->type = type;
    // Trees give more hits than rocks
    res->maxHealth = (type == RESOURCE_TREE) ? 5 : 3;
    res->health = res->maxHealth;
    res->shakeTimer = 0.0f;
    world->resourceCount++;
    return true;
}

void SpawnResources(World* world, int count) {
    float worldPixelWidth = world->width * TILE_SIZE;
    float worldPixelHeight = world->height * TILE_SIZE;

    // Keep a safe zone around center (where player/heart spawns)
    float centerX = worldPixelWidth / 2.0f;
    float centerY = worldPixelHeight / 2.0f;
    float safeZone = 200.0f;

    for (int i = 0; i < count && world->resourceCount < MAX_RESOURCES; i++) {
        // Random position
        float x = (float)(rand() % (int)worldPixelWidth);
        float y = (float)(rand() % (int)worldPixelHeight);

        // Skip if too close to center
        float dx = x - centerX;
        float dy = y - centerY;
        if (sqrtf(dx * dx + dy * dy) < safeZone) {
            continue;
        }

        // Random type (50/50 tree or rock)
        ResourceType type = (rand() % 2 == 0) ? RESOURCE_TREE : RESOURCE_ROCK;
        Vector2 position = {x, y};

        // Try to spawn the main resource
        if (TrySpawnResource(world, position, type)) {
            // Spawn companion resource (opposite type) nearby
            ResourceType companionType = (type == RESOURCE_TREE) ? RESOURCE_ROCK : RESOURCE_TREE;

            // Try a few random angles to find a valid spot
            // Random distance: 60-150 pixels apart (some close, some far)
            float pairDistance = 60.0f + (float)(rand() % 90);

            for (int attempt = 0; attempt < 4; attempt++) {
                float angle = (float)(rand() % 360) * 3.14159f / 180.0f;
                Vector2 companionPos = {
                    position.x + cosf(angle) * pairDistance,
                    position.y + sinf(angle) * pairDistance
                };

                // Check it's not in the safe zone
                float cdx = companionPos.x - centerX;
                float cdy = companionPos.y - centerY;
                if (sqrtf(cdx * cdx + cdy * cdy) < safeZone) {
                    continue;
                }

                if (TrySpawnResource(world, companionPos, companionType)) {
                    break;  // Successfully spawned companion
                }
            }
        }
    }
}

void UpdateResources(World* world, float deltaTime) {
    for (int i = 0; i < world->resourceCount; i++) {
        Resource* res = &world->resources[i];
        if (res->shakeTimer > 0.0f) {
            res->shakeTimer -= deltaTime;
            if (res->shakeTimer < 0.0f) res->shakeTimer = 0.0f;
        }
    }
}

void DrawResources(const World* world) {
    for (int i = 0; i < world->resourceCount; i++) {
        const Resource* res = &world->resources[i];

        // Calculate shake offset
        float shakeOffset = 0.0f;
        if (res->shakeTimer > 0.0f) {
            shakeOffset = sinf(res->shakeTimer * 50.0f) * 4.0f;
        }
        Vector2 drawPos = {res->position.x + shakeOffset, res->position.y};

        if (res->type == RESOURCE_TREE) {
            // Draw tree - brown trunk with green top
            DrawCircleV(drawPos, res->radius * 0.3f, BROWN);
            DrawCircleV(drawPos, res->radius, DARKGREEN);
        } else {
            // Draw rock - gray circle
            DrawCircleV(drawPos, res->radius, GRAY);
            // Add some highlight
            Vector2 highlight = {drawPos.x - res->radius * 0.3f, drawPos.y - res->radius * 0.3f};
            DrawCircleV(highlight, res->radius * 0.3f, LIGHTGRAY);
        }
    }
}

// Returns: 0=nothing gathered, 1=wood, 2=stone
int GatherResource(World* world, Vector2 position, float reach) {
    int closestIndex = -1;
    float closestDist = reach;

    // Find closest resource within reach
    for (int i = 0; i < world->resourceCount; i++) {
        Resource* res = &world->resources[i];
        float dx = position.x - res->position.x;
        float dy = position.y - res->position.y;
        float dist = sqrtf(dx * dx + dy * dy);

        if (dist < closestDist) {
            closestDist = dist;
            closestIndex = i;
        }
    }

    if (closestIndex == -1) {
        return 0;  // Nothing in range
    }

    Resource* res = &world->resources[closestIndex];
    int result = (res->type == RESOURCE_TREE) ? 1 : 2;

    // Trigger shake animation
    res->shakeTimer = 0.3f;

    // Reduce health
    res->health--;

    // Remove if depleted and spawn a new one somewhere else
    if (res->health <= 0) {
        ResourceType depletedType = res->type;

        // Remove by swapping with last
        world->resources[closestIndex] = world->resources[world->resourceCount - 1];
        world->resourceCount--;

        // Try to spawn a replacement at a random location
        float worldPixelWidth = world->width * TILE_SIZE;
        float worldPixelHeight = world->height * TILE_SIZE;
        float centerX = worldPixelWidth / 2.0f;
        float centerY = worldPixelHeight / 2.0f;
        float safeZone = 200.0f;

        for (int attempt = 0; attempt < 20; attempt++) {
            float x = (float)(rand() % (int)worldPixelWidth);
            float y = (float)(rand() % (int)worldPixelHeight);

            float dx = x - centerX;
            float dy = y - centerY;
            if (sqrtf(dx * dx + dy * dy) < safeZone) continue;

            Vector2 newPos = {x, y};
            if (TrySpawnResource(world, newPos, depletedType)) {
                break;
            }
        }
    }

    return result;
}
