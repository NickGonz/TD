#ifndef WORLD_H
#define WORLD_H

#include "raylib.h"

#define WORLD_WIDTH 25
#define WORLD_HEIGHT 19
#define TILE_SIZE 32

enum TileType {
    TILE_EMPTY,
    TILE_WALL
};

struct World {
    TileType tiles[WORLD_HEIGHT][WORLD_WIDTH];
    int width;
    int height;
    int tileSize;
};

void InitWorld(World* world);
void DrawWorld(const World* world);
bool IsTileWalkable(const World* world, int gridX, int gridY);
void WorldToGrid(float worldX, float worldY, int* gridX, int* gridY);

#endif // WORLD_H
