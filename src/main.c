#include <stdint.h>
#include <math.h>

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

#define INIT_WORLD_LAYER(value) { [0 ... WORLD_LENGTH-1] = { [0 ... WORLD_BREADTH-1] = value } }
int world[WORLD_HEIGHT][WORLD_LENGTH][WORLD_BREADTH] = {
	[0] = INIT_WORLD_LAYER(1), // Bedrock
	[1] = INIT_WORLD_LAYER(2), // Stone
	[2] = INIT_WORLD_LAYER(2), // Stone
	[3] = INIT_WORLD_LAYER(3), // Dirt
	[4] = INIT_WORLD_LAYER(3), // Dirt
	[5] = INIT_WORLD_LAYER(4)  // Grass
};

Camera camera = { 0 };

typedef struct {
	int x;
	int y;
	int z;
} Vector3Int;
Vector3Int selected = { 0, 0, 0 };

/*
 * Return what block is under the player's feet
 */
int blockUnderPlayer() {
	float feetY = camera.position.y - PLAYER_HEIGHT;
	int x = (int)floor(camera.position.x);
	int y = (int)floor(feetY);
	int z = (int)floor(camera.position.z);

	if (x < 0 || x >= WORLD_LENGTH || z < 0 || z >= WORLD_BREADTH || y < 0 || y >= WORLD_HEIGHT)
		return 0;

	return world[y][x][z];
}

/*
 * If player is colliding with a block.
 */
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

int main(void) {
	int width = WINDOW_WIDTH;
	int height = WINDOW_HEIGHT;
	InitWindow(width, height, "Mycraft");

	uint8_t debugMode = 0;

	camera.position   = (Vector3){ 4.0f, (WORLD_HEIGHT + PLAYER_HEIGHT), 4.0f };
	camera.target     = (Vector3){ 0.0f, 2.0f, 0.0f };
	camera.up         = (Vector3){ 0.0f, 1.0f, 0.0f };
	camera.fovy       = 60.0f;
	camera.projection = CAMERA_PERSPECTIVE;

	DisableCursor();
	SetMousePosition(width/2, height/2);
	uint8_t isHolding = 0;
	float holdTime    = 0.0f;

	const float gravity      = 6.9f;
	const float jumpSpeed    = 5.0f;
	float verticalVelocity   = 0.0f;

	SetTargetFPS(30);

	while (!WindowShouldClose()) {
		// Fullscreen toggle
		if (IsKeyPressed(KEY_ENTER) && (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT))) {
			int display = GetCurrentMonitor();
			if (IsWindowFullscreen()) {
				width = WINDOW_WIDTH;
				height = WINDOW_HEIGHT;
			}
			else {
				width = GetMonitorWidth(display);
				height = GetMonitorHeight(display);
			}
			SetWindowSize(width, height);
			ToggleFullscreen();
		}

		// Debug toggle
		if (IsKeyPressed(KEY_F3)) debugMode = !debugMode;

		// Mouse click
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			if (!isHolding) {
				isHolding = 1;
				holdTime = 0.0f;
			} else {
				holdTime += GetFrameTime();
				if (holdTime >= 1.0f) { // 1 second
										// Break the block
					world[selected.y][selected.x][selected.z] = 0;
					isHolding = 0;
				}
			}
		} else isHolding = 0;

		int onGround = (blockUnderPlayer() != 0);

		// Jumping
		if (IsKeyPressed(KEY_SPACE) && onGround) {
			verticalVelocity = jumpSpeed;
			onGround = 0;
		}

		// Gravity
		if (!onGround) {
			camera.position.y += verticalVelocity * GetFrameTime();
			verticalVelocity -= gravity * GetFrameTime();
		}

		Vector3 oldPosition = camera.position;
        UpdateCamera(&camera, CAMERA_FIRST_PERSON);

        if (playerColliding(camera.position))
            camera.position = oldPosition;

		BeginDrawing();

		ClearBackground(SKYBLUE);

		BeginMode3D(camera);

		// Player hitbox (debug)
		if(debugMode) {
			DrawCubeWires(camera.position, PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_WIDTH, DARKPURPLE);
		}

		// Block selection
		Ray crosshairRay              = GetScreenToWorldRay((Vector2){ width/2, height/2 }, camera);
		RayCollision closestCollision = { 0 };
		closestCollision.distance     = INFINITY;
		Vector3Int newSelected        = { -1, -1, -1 };
		const float maxRange          = 5.0f;

		for (int depth = 0; depth < WORLD_HEIGHT; ++depth) {
			for (int length = 0; length < WORLD_LENGTH; ++length) {
				for (int breath = 0; breath < WORLD_BREADTH; ++breath) {
					if (world[depth][length][breath] < 1) continue;

					Vector3 pos = (Vector3){ (float)length, (float)depth, (float)breath };
					BoundingBox box = {
						.min = { pos.x - 0.5f, pos.y - 0.5f, pos.z - 0.5f },
						.max = { pos.x + 0.5f, pos.y + 0.5f, pos.z + 0.5f }
					};

					RayCollision col = GetRayCollisionBox(crosshairRay, box);

					if (col.hit && col.distance <= maxRange && col.distance < closestCollision.distance) {
						closestCollision = col;
						newSelected.x = length;
						newSelected.y = depth;
						newSelected.z = breath;
					}
				}
			}
		}

		if (closestCollision.hit) {
			selected = newSelected;
		} else {
			selected.x = -1;
		}

		for (int depth = 0; depth < WORLD_HEIGHT; ++depth) {
			for (int length = 0; length < WORLD_LENGTH; ++length) {
				for (int breath = 0; breath < WORLD_BREADTH; ++breath) {
					if(world[depth][length][breath] < 1) continue;

					Color color;
					switch(world[depth][length][breath]) {
						case 1:
							// Bedrock
							color = BLACK;
							break;
						case 2:
							// Stone
							color = GRAY;
							break;
						case 3:
							// Dirt
							color = BROWN;
							break;
						case 4:
							// Grass
							color = DARKGREEN;
							break;
						default:
							// Grass
							color = DARKGREEN;
							break;
					}

					Vector3 pos = (Vector3){ (float)length, (float)depth, (float)breath };

					DrawCube(pos, BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, color);

					if (length == selected.x && depth == selected.y && breath == selected.z) {
						DrawCubeWires(pos, 1.0f, 1.0f, 1.0f, WHITE);
					} else {
						DrawCubeWires(pos, 1.0f, 1.0f, 1.0f, BLACK);
					}
				}
			}
		}

		EndMode3D();

		// Crosshair
		DrawLine(width / 2 - 10, height / 2, width / 2 + 10, height / 2, RED);
		DrawLine(width / 2, height / 2 - 10, width / 2, height / 2 + 10, RED);

		// Debug
		if(debugMode) {
			DrawText(TextFormat("Selected: %d, %d, %d", selected.x, selected.y, selected.z), 20, 10, 10, BLACK);
			DrawText(TextFormat("Camera position: %.2f, %.2f, %.2f", camera.position.x, camera.position.y, camera.position.z), 20, 20, 10, BLACK);
			DrawText(TextFormat("Camera target: %.2f, %.2f, %.2f", camera.target.x, camera.target.y, camera.target.z), 20, 30, 10, BLACK);
			DrawText(TextFormat("Mouse holding: %d", isHolding), 20, 40, 10, BLACK);
			DrawText(TextFormat("Mouse holding duration: %.2f", holdTime), 20, 50, 10, BLACK);
			DrawText(TextFormat("Block under: %d", blockUnderPlayer()), 20, 60, 10, BLACK);
			DrawText(TextFormat("Player colliding: %d", playerColliding()), 20, 70, 10, BLACK);
		}

		EndDrawing();
	}

	CloseWindow();

	return 0;
}
