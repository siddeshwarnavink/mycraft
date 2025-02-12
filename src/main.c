#include "raylib.h"
#include "rcamera.h"

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

		// Basic world-gen
		// l0 - Bedrock - BLACK
		// l1-3 - Stone, Ores - GREY
		// l4-6, Dirt, Grass - BROWN, GREEN

		Ray crosshairRay = GetScreenToWorldRay((Vector2){ width/2, height/2 }, camera);

		for (int l = 0; l <= 6; ++l) {
			for (float i = 0.0f; i < 20.0f; i += 1.0f) {
				for (float j = 0.0f; j < 20.0f; j += 1.0f) {
					Color color = BROWN;
					if (l == 0)
						color = BLACK;
					else if(l > 0 && l < 4)
						color = GRAY;
					else if(l == 6)
						color = DARKGREEN;

					Vector3 pos = (Vector3){ i, l, j };
					const float size = 1.0f;

					DrawCube(pos, size, size, size, color);

					RayCollision col = GetRayCollisionBox(crosshairRay,
							(BoundingBox){(Vector3){ pos.x - size/4, pos.y - size/4, pos.z - size/4 },
							(Vector3){ pos.x + size/4, pos.y + size/4, pos.z + size/4 }});

					if(col.hit) {
						DrawCubeWires(pos, size, size, size, WHITE);
					}
				}
			}
		}

		EndMode3D();

		// Crosshair
		DrawLine(width / 2 - 10, height / 2, width / 2 + 10, height / 2, RED);
		DrawLine(width / 2, height / 2 - 10, width / 2, height / 2 + 10, RED);

		EndDrawing();
	}

	CloseWindow();

	return 0;
}
