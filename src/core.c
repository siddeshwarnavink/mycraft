#include <math.h>

#include "core.h"
#include "state.h"

int blockUnderPlayer() {
	float feetY = camera.position.y - PLAYER_HEIGHT;
	int x = (int)floor(camera.position.x);
	int y = (int)floor(feetY);
	int z = (int)floor(camera.position.z);

	if (x < 0 || x >= WORLD_LENGTH || z < 0 || z >= WORLD_BREADTH || y < 0 || y >= WORLD_HEIGHT)
		return 0;

	return world[y][x][z];
}

uint8_t playerColliding() {
	for (int depth = 0; depth < WORLD_HEIGHT; ++depth) {
		for (int length = 0; length < WORLD_LENGTH; ++length) {
			for (int breath = 0; breath < WORLD_BREADTH; ++breath) {
				if(world[depth][length][breath] < 1) continue;

				Vector3 block = (Vector3){ (float)length, (float)depth, (float)breath };
				BoundingBox blockBox = (BoundingBox){
					.min = { block.x - BLOCK_SIZE / 2, block.y - BLOCK_SIZE / 2, block.z - BLOCK_SIZE / 2 },
						.max = { block.x + BLOCK_SIZE / 2, block.y + BLOCK_SIZE / 2, block.z + BLOCK_SIZE / 2 }
				};

				Vector3 player = camera.position;
				BoundingBox playerBox = (BoundingBox){
					.min = { player.x - PLAYER_WIDTH / 2, player.y - PLAYER_HEIGHT, player.z - PLAYER_WIDTH / 2 },
						.max = { player.x + PLAYER_WIDTH / 2, player.y, player.z + PLAYER_WIDTH / 2 }
				};

				if (CheckCollisionBoxes(playerBox, blockBox)) {
					return 1;
				}

			}
		}
	}
	return 0;
}

void handleGravity() {
	int onGround = (blockUnderPlayer() != 0);

	// Jumping
	if (IsKeyPressed(KEY_SPACE) && onGround) {
		verticalVelocity = JUMP_SPEED;
		onGround = 0;
	}

	// Gravity
	if (!onGround) {
		camera.position.y += verticalVelocity * GetFrameTime();
		verticalVelocity -= GRAVITY * GetFrameTime();
	}
}

void handleCollision() {
	Vector3 oldPosition = camera.position;
	UpdateCamera(&camera, CAMERA_FIRST_PERSON);

	if (playerColliding())
		camera.position = oldPosition;
}
