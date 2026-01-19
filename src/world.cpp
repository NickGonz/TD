#include "world.h"

void InitWorld(World* world) {
    world->width = WORLD_WIDTH;
    world->height = WORLD_HEIGHT;
    world->tileSize = TILE_SIZE;

    for (int y = 0; y < WORLD_HEIGHT; y++) {
        for (int x = 0; x < WORLD_WIDTH; x++) {
            world->tiles[y][x] = TILE_EMPTY;
        }
    }

    for (int x = 0; x < WORLD_WIDTH; x++) {
        world->tiles[0][x] = TILE_WALL; // top
        world->tiles[WORLD_HEIGHT - 1][x] = TILE_WALL; // bottom
    }

    for (int y = 0; y < WORLD_HEIGHT; y++) {
        world->tiles[y][0] = TILE_WALL; // left
        world->tiles[y][WORLD_WIDTH - 1] = TILE_WALL; // right
    }


}

void DrawWorld(const World* world) {
    for (int y = 0; y < WORLD_HEIGHT; y++) {
        for (int x = 0; x < WORLD_WIDTH; x++) {
            int pixelX = x * world->tileSize;
            int pixelY = y * world->tileSize;

            if (world->tiles[y][x] == TILE_WALL) {
                DrawRectangle(pixelX, pixelY, world->tileSize, world->tileSize, DARKBROWN);
            }

            DrawRectangleLines(pixelX, pixelY, world->tileSize, world->tileSize, LIGHTGRAY);
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
