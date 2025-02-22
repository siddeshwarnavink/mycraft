#ifndef BLOCKS_H
#define BLOCKS_H

enum BlockType {
    B_VOID = 0,
    B_BEDROCK,
    B_STONE,
    B_DIRT,
    B_GRASS
};

void drawBlock(enum BlockType block, Vector3 position);

#endif // !BLOCKS_H
