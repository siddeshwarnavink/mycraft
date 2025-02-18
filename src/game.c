#include <math.h>

#include "raylib.h"

#include "game.h"
#include "core.h"
#include "state.h"
#include "blocks.h"

static void _block_selection() {
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
}

static void _render_world() {
	for (int depth = 0; depth < WORLD_HEIGHT; ++depth) {
		for (int length = 0; length < WORLD_LENGTH; ++length) {
			for (int breath = 0; breath < WORLD_BREADTH; ++breath) {
				if(world[depth][length][breath] < 1) continue;

				Vector3 pos = (Vector3){ (float)length, (float)depth, (float)breath };

				if (length == selected.x && depth == selected.y && breath == selected.z) {
					DrawCubeWires(pos, 1.0f, 1.0f, 1.0f, WHITE);
				} else {
					DrawCubeWires(pos, 1.0f, 1.0f, 1.0f, BLACK);
				}

				if(world[depth][length][breath] == 4) {
					drawGrass(pos);
					continue;
				}

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
				DrawCube(pos, BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, color);
			}
		}
	}

}

void gameLoop() {
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

	handleGravity();
	handleCollision();

	BeginDrawing();

	ClearBackground(SKYBLUE);

	BeginMode3D(camera);

	// Player hitbox
	if(debugMode)
		DrawCubeWires(camera.position, PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_WIDTH, DARKPURPLE);


	_block_selection();

	_render_world();

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
