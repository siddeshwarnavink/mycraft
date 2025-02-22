#include "state.h"

int width = WINDOW_WIDTH;
int height = WINDOW_HEIGHT;

#define INIT_WORLD_LAYER(value) { [0 ... WORLD_LENGTH-1] = { [0 ... WORLD_BREADTH-1] = value } }
enum BlockType world[WORLD_HEIGHT][WORLD_LENGTH][WORLD_BREADTH] = {
    [0] = INIT_WORLD_LAYER(B_BEDROCK),
    [1] = INIT_WORLD_LAYER(B_STONE),
    [2] = INIT_WORLD_LAYER(B_STONE),
    [3] = INIT_WORLD_LAYER(B_STONE),
    [4] = INIT_WORLD_LAYER(B_DIRT),
    [5] = INIT_WORLD_LAYER(B_GRASS)
};

Vector3Int selected = { 0, 0, 0 };

Camera camera = {
    .position = { 4.0f, (WORLD_HEIGHT + PLAYER_HEIGHT), 4.0f },
    .target = { 0.0f, 2.0f, 0.0f },
    .up = { 0.0f, 1.0f, 0.0f },
    .fovy = 60.0f,
    .projection = CAMERA_PERSPECTIVE
};

uint8_t debugMode = 0;

uint8_t isHolding = 0;
float holdTime    = 0.0f;

float verticalVelocity = 0.0f;

int hudPos = 0;

Texture2D texture;

Inventory inv = {0};
