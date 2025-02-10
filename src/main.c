#include "raylib.h"

int main(void) {
    InitWindow(600, 400, "Mycraft");

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(RAYWHITE);
        DrawText("Hello World!", 190, 200, 20, LIGHTGRAY);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
