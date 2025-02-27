#ifndef BLOCKS_H
#define BLOCKS_H

#include "raylib.h"

#include "utils.h"

enum BlockType {
    B_VOID = 0,
    B_BEDROCK,
    B_STONE,
    B_DIRT,
    B_GRASS
};

/*
 * Draw block for world.
 */
void drawBlock(enum BlockType block, Vector3 position);

/*
 * Draw block for HUD.
 */
void drawHudBlock(enum BlockType block, Vector3 position);

/*
 * Draw block for hand held.
 */
void drawHandBlock(enum BlockType block, Vector3 position);

#endif // !BLOCKS_H
