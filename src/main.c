#include <stdint.h>

#include "raylib.h"
#include "rcamera.h"

#define WORLD_LENGTH 20
#define WORLD_BREADTH 20
#define WORLD_HEIGHT 6

#define INIT_WORLD_LAYER(value) { [0 ... WORLD_LENGTH-1] = { [0 ... WORLD_BREADTH-1] = value } }
int world[WORLD_HEIGHT][WORLD_LENGTH][WORLD_BREADTH] = {
    [0] = INIT_WORLD_LAYER(1), // Bedrock
    [1] = INIT_WORLD_LAYER(2), // Stone
    [2] = INIT_WORLD_LAYER(2), // Stone
    [3] = INIT_WORLD_LAYER(3), // Dirt
    [4] = INIT_WORLD_LAYER(3), // Dirt
    [5] = INIT_WORLD_LAYER(4)  // Grass
};

typedef struct {
	int x;
	int y;
	int z;
} Vector3Int;

Vector3Int selected = { 0, 0, 0 };

int main(void) {
	const int width = 800;
	const int height = 450;
	InitWindow(width, height, "Mycraft");

	Camera camera     = { 0 };
	camera.position   = (Vector3){ 4.0f, 8.0f, 4.0f };
	camera.target     = (Vector3){ 0.0f, 2.0f, 0.0f };
	camera.up         = (Vector3){ 0.0f, 1.0f, 0.0f };
	camera.fovy       = 60.0f;
	camera.projection = CAMERA_PERSPECTIVE;

	DisableCursor();
	SetMousePosition(width/2, height/2);
	uint8_t isHolding = 0;
	float holdTime    = 0.0f;

	const float gravity      = 0.69f;
	const float jumpSpeed    = 8.0f;
	float verticalVelocity   = 0.0f;
	const float groundHeight = 8.0f;
	uint8_t isJumping        = 0;

	SetTargetFPS(30);

	while (!WindowShouldClose()) {
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

		// Jump
		if (IsKeyPressed(KEY_SPACE) && !isJumping) {
            isJumping = 1;
            verticalVelocity = jumpSpeed;
        }
        if (isJumping) {
            camera.position.y += verticalVelocity * GetFrameTime();
            verticalVelocity -= gravity;

            if (camera.position.y <= groundHeight) {
                camera.position.y = groundHeight;
                isJumping = 0;
                verticalVelocity = 0.0f;
            }
        }

		UpdateCamera(&camera, CAMERA_FIRST_PERSON);

		BeginDrawing();

		ClearBackground(RAYWHITE);

		BeginMode3D(camera);

		Ray crosshairRay = GetScreenToWorldRay((Vector2){ width/2, height/2 }, camera);

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
					const float size = 1.0f;

					DrawCube(pos, size, size, size, color);

					RayCollision col = GetRayCollisionBox(crosshairRay,
                                                          (BoundingBox){(Vector3){ pos.x - size/4, pos.y - size/4, pos.z - size/4 },
                                                                        (Vector3){ pos.x + size/4, pos.y + size/4, pos.z + size/4 }});

					if(col.hit) {
						selected.x = length;
						selected.y = depth;
						selected.z = breath;
						DrawCubeWires(pos, size, size, size, WHITE);
					} else {
						DrawCubeWires(pos, size, size, size, BLACK);
					}
				}
			}
		}

		EndMode3D();

		// Crosshair
		DrawLine(width / 2 - 10, height / 2, width / 2 + 10, height / 2, RED);
		DrawLine(width / 2, height / 2 - 10, width / 2, height / 2 + 10, RED);

		// Debug
		// DrawText(TextFormat("Selected: %d, %d, %d", selected.x, selected.y, selected.z), 20, 20, 15, BLACK);
		// DrawText(TextFormat("Camera position: %.2f, %.2f, %.2f", camera.position.x, camera.position.y, camera.position.z), 20, 40, 15, BLACK);
		// DrawText(TextFormat("Camera target: %.2f, %.2f, %.2f", camera.target.x, camera.target.y, camera.target.z), 20, 60, 15, BLACK);
		// DrawText(TextFormat("isHolding: %d", isHolding), 20, 40, 15, BLACK);
		// DrawText(TextFormat("isHolding: %.2f", holdTime), 20, 60, 15, BLACK);

		EndDrawing();
	}

	CloseWindow();

	return 0;
}
