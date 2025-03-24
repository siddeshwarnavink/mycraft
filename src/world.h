/*
 * World generation and rendering.
 */
#ifndef WORLD_H
#define WORLD_H

#include "blocks.h"

#define WORLD_LENGTH  15
#define WORLD_BREADTH 15
#define WORLD_HEIGHT  15
#define BLOCK_SIZE 1.0f

typedef struct {
    enum BlockType blocks[WORLD_HEIGHT][WORLD_LENGTH][WORLD_BREADTH];
} World;

World getWorld();

void initWorld();

/*
 * Update block.
 */
void updateWorldBlock(int x, int y, int z, enum BlockType b);

/*
 * Render the world as we see it.
 */
void renderWorld();

void freeWorld();

#endif // !WORLD_H
