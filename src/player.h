/*
 * Core game logic like gravity and collision.
 */
#ifndef PLAYER_H
#define PLAYER_H

#include <stdint.h>

#include "raylib.h"

#include "utils.h"

#define PLAYER_HEIGHT 2
#define PLAYER_WIDTH  0.35f

#define GRAVITY 6.9f
#define JUMP_SPEED 4.20f

typedef struct {
    Camera camera;
    float verticalVelocity;
    Vector3Int blockSelected;
} Player;

void initPlayer();

void freePlayer();

Player getPlayer();

/*
 * Set the block selected by the player.
 */
void setBlockSelected(Vector3Int s);

/*
 * Renders player's right hand if not holding a block.
 * If holding a block render the block.
 */
void renderPlayer();

/*
 * Return what block is under the player's feet
 */
extern enum BlockType blockUnderPlayer();


/*
 * If player is colliding with a block.
 */
extern uint8_t playerColliding(void);

/*
 * Player gravity logic.
 */
extern void handleGravity(void);

/*
 * Player collision logic
 */
extern void handleCollision(void);

#endif // !PLAYER_H
