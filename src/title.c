#include <math.h>

#include "raylib.h"

#include "title.h"
#include "window.h"
#include "screen.h"
#include "sounds.h"

Texture2D splash;

float _clamp(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

void initTitle() {
	splash = LoadTexture("resources/images/splash.jpg");
}

void freeTitle() {
	UnloadTexture(splash);
}

void titleLoop() {
    static float titleTimer = 0.0f;
    titleTimer += GetFrameTime();

	if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) {
		setScreen(GAMEPLAY);
		playSound(S_CLICK);
	}

    BeginDrawing();
    ClearBackground(RAYWHITE);

	DrawTexture(splash, 0, 0, WHITE);

	float titleY = 30;
    if (titleTimer < 1.0f) {
        float t = titleTimer / 1.0f;
        t = 1 - powf(1 - t, 4);
        titleY = -100 + t * (30 - (-100));
    }

    DrawText("MyCraft", GetScreenWidth()/2 - 100, titleY, 60, GREEN);

	float subtitleAlpha = _clamp((titleTimer - 0.5f) * 2.0f, 0.0f, 1.0f);
    Color subtitleColor = Fade(WHITE, subtitleAlpha);
    DrawText("Buggy game by Siddeshwar", GetScreenWidth()/2 - 120, 100, 20, subtitleColor);

	if (titleTimer > 1.5f) {
        float blink = (sinf(titleTimer * 5.0f) + 1.0f) / 2.0f;
        Color enterColor = Fade(WHITE, blink);
        DrawText("Press ENTER to play.", GetScreenWidth()/2 - 120, 300, 20, enterColor);
    }

    EndDrawing();
}
