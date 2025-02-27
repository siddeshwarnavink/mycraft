#include "world.h"
#include "utils.h"
#include "player.h"

static World w;

void initWorld() {
	for (int i = 0; i < WORLD_HEIGHT; ++i) {
		enum BlockType value = B_VOID;
		if (i == 0)        value = B_BEDROCK;
		else if (i <= 3)   value = B_STONE;
		else if (i == 4)   value = B_DIRT;
		else if (i == 5)   value = B_GRASS;

		for (int j = 0; j < WORLD_LENGTH; ++j) {
			for (int k = 0; k < WORLD_BREADTH; ++k) {
				w.blocks[i][j][k] = value;
			}
		}
	}
}

void renderWorld() {
    for (int depth = 0; depth < WORLD_HEIGHT; ++depth) {
        for (int length = 0; length < WORLD_LENGTH; ++length) {
            for (int breath = 0; breath < WORLD_BREADTH; ++breath) {
                if(w.blocks[depth][length][breath] == B_VOID) continue;

                Vector3 pos = (Vector3){ (float)length, (float)depth, (float)breath };

                if (length == getPlayer().blockSelected.x && depth == getPlayer().blockSelected.y && breath == getPlayer().blockSelected.z) {
                    DrawCubeWires(pos, 1.0f, 1.0f, 1.0f, WHITE);
                } else {
                    DrawCubeWires(pos, 1.0f, 1.0f, 1.0f, BLACK);
                }

                drawBlock(w.blocks[depth][length][breath], pos);
            }
        }
    }
}

void updateWorldBlock(int x, int y, int z, enum BlockType b) {
    w.blocks[x][y][z] = b;
}

World getWorld() {
    return w;
}

void freeWorld() {
    // :)
}
