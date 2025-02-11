#include "raylib.h"
#include "rcamera.h"

int main(void) {
    InitWindow(800, 450, "Mycraft");

	Camera camera     = { 0 };
	camera.position   = (Vector3){ 4.0f, 8.0f, 4.0f };
	camera.target     = (Vector3){ 0.0f, 2.0f, 0.0f };
	camera.up         = (Vector3){ 0.0f, 1.0f, 0.0f };
	camera.fovy       = 60.0f;
	camera.projection = CAMERA_PERSPECTIVE;

    DisableCursor();

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

					DrawCube((Vector3){ i, l, j }, 1, 1, 1, color);
				}
			}
		}

        EndMode3D();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
