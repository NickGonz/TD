#ifndef WORLD_H
#define WORLD_H

#include "raylib.h"
#include "tower.h"
#include "resource.h"

#define WORLD_WIDTH 200
#define MAX_TOWERS 500
#define WORLD_HEIGHT 150
#define TILE_SIZE 32
#define SPRITE_SIZE 16  // Visual tile size in spritesheet

enum TileType {
    TILE_EMPTY,
    TILE_WALL
};

struct World {
    TileType tiles[WORLD_HEIGHT][WORLD_WIDTH];
    int width;
    int height;
    int tileSize;

    // Visual layer - 2x resolution (each gameplay tile = 2x2 visual tiles)
    int visualTiles[WORLD_HEIGHT * 2][WORLD_WIDTH * 2];
    int decorationTiles[WORLD_HEIGHT * 2][WORLD_WIDTH * 2];  // -1 = no decoration

    // Tower storage
    Tower towers[MAX_TOWERS];
    int towerCount;

    // Resource storage (trees, rocks)
    Resource resources[MAX_RESOURCES];
    int resourceCount;

    // Spritesheet
    Texture2D tileset;
};

void InitWorld(World* world);
void LoadWorldAssets(World* world);
void UnloadWorldAssets(World* world);
void DrawWorld(const World* world);
void DrawTowers(const World* world);
bool IsTileWalkable(const World* world, int gridX, int gridY);
bool CheckTowerCollision(const World* world, Vector2 position, float radius);
bool CheckResourceCollision(const World* world, Vector2 position, float radius);
bool CheckAnyCollision(const World* world, Vector2 position, float radius);
bool IsInWorldBounds(const World* world, Vector2 position, float radius);
bool PlaceTower(World* world, Vector2 position, TowerType type);
void SpawnResources(World* world, int count);
void UpdateResources(World* world, float deltaTime);
void DrawResources(const World* world);
int GatherResource(World* world, Vector2 position, float reach);  // Returns: 0=nothing, 1=wood, 2=stone
void WorldToGrid(float worldX, float worldY, int* gridX, int* gridY);

#endif // WORLD_H
