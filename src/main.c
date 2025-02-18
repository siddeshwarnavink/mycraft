#include "raylib.h"

#include "game.h"
#include "state.h"

int main(void) {
	width = WINDOW_WIDTH;
	height = WINDOW_HEIGHT;
	InitWindow(width, height, "Mycraft");

	DisableCursor();
	SetMousePosition(width/2, height/2);

	SetTargetFPS(30);

	while (!WindowShouldClose()) {
		// Fullscreen toggle
		if (IsKeyPressed(KEY_ENTER) && (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT))) {
			const int display = GetCurrentMonitor();
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

		gameLoop();
	}

	CloseWindow();

	return 0;
}
