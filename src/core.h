/*
 * Core game logic like gravity and collision.
 */
#ifndef CORE_H
#define CORE_H

#include <stdint.h>

/*
 * Return what block is under the player's feet
 */
extern int blockUnderPlayer(void);

/*
 * If player is colliding with a block.
 */
extern uint8_t playerColliding(void);

extern void handleGravity(void);

extern void handleCollision(void);

#endif // !CORE_H
