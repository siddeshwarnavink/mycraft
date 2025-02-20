#include <stdlib.h>
#include <time.h>

#include "raylib.h"

#include "game.h"
#include "state.h"
#include "sounds.h"

int main(void) {
	srand(time(NULL));

	width = WINDOW_WIDTH;
	height = WINDOW_HEIGHT;
	InitWindow(width, height, "Mycraft");

	InitAudioDevice();
	loadSounds();
	playSound(S_BACKGROUND1);

	DisableCursor();
	SetMousePosition(width/2, height/2);

	SetTargetFPS(30);

	texture = LoadTexture("resources/texture.png");

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
	UnloadTexture(texture);
	unloadSounds();
	CloseAudioDevice();

	return 0;
}
