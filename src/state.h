/*
 * Game constants and runtime state.
 */
#ifndef STATE_H
#define STATE_H

#include <stdint.h>

#include "raylib.h"
#include "rcamera.h"

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 450

#define WORLD_LENGTH  20
#define WORLD_BREADTH 20
#define WORLD_HEIGHT  6
#define BLOCK_SIZE 1.0f

#define PLAYER_HEIGHT 2
#define PLAYER_WIDTH  0.35f

#define GRAVITY 6.9f
#define JUMP_SPEED 4.20f

typedef struct {
	int x;
	int y;
	int z;
} Vector3Int;

// Current resolition of window
extern int width;
extern int height;

extern int world[WORLD_HEIGHT][WORLD_LENGTH][WORLD_BREADTH];
extern Camera camera;
extern Vector3Int selected;

extern uint8_t debugMode;

// Left mouse button
extern uint8_t isHolding;
extern float holdTime;

// Gravity & jumping
extern float verticalVelocity;

#endif // !STATE_H
