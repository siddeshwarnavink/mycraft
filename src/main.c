#include "raylib.h"
#include "rcamera.h"

#define WORLD_LENGTH 20
#define WORLD_BREADTH 20
#define WORLD_HEIGHT 6

// 0 - void
// 1 - bedrock
// 2 - stone
// 3 - dirt
// 4 - grass
#define INIT_WORLD_LAYER(value) { [0 ... WORLD_LENGTH-1] = { [0 ... WORLD_BREADTH-1] = value } }
#define INIT_WORLD(value) { [0 ... WORLD_HEIGHT-1] = INIT_WORLD_LAYER(value) }
const int world[WORLD_HEIGHT][WORLD_LENGTH][WORLD_BREADTH] = INIT_WORLD(3);

Vector3 selected = { 0 };

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

	SetTargetFPS(30);

	while (!WindowShouldClose()) {
		UpdateCamera(&camera, CAMERA_FIRST_PERSON);

		BeginDrawing();

		ClearBackground(RAYWHITE);

		BeginMode3D(camera);

		Ray crosshairRay = GetScreenToWorldRay((Vector2){ width/2, height/2 }, camera);

		for (int depth = 0; depth < WORLD_HEIGHT; ++depth) {
			for (int length = 0; length < WORLD_LENGTH; ++length) {
				for (int breath = 0; breath < WORLD_BREADTH; ++breath) {
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
						selected = pos;
						DrawCubeWires(pos, size, size, size, WHITE);
					}
				}
			}
		}

		EndMode3D();

		// Crosshair
		DrawLine(width / 2 - 10, height / 2, width / 2 + 10, height / 2, RED);
		DrawLine(width / 2, height / 2 - 10, width / 2, height / 2 + 10, RED);

		// Debug
		DrawText(TextFormat("Selected: %.1f, %.1f, %.1f", selected.x, selected.y, selected.z), 20, 20, 15, BLACK);

		EndDrawing();
	}

	CloseWindow();

	return 0;
}
