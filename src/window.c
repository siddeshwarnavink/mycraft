#include "raylib.h"

#include "window.h"

static GameWindow gw;

void initWindow() {
    gw.width = WINDOW_WIDTH;
    gw.height = WINDOW_HEIGHT;

    InitWindow(gw.width, gw.height, "Mycraft");
    DisableCursor();
    SetMousePosition(gw.width/2, gw.height/2);
    SetTargetFPS(30);
}

void toggleFullscreen() {
    const int display = GetCurrentMonitor();
    if (IsWindowFullscreen()) {
        gw.width = WINDOW_WIDTH;
        gw.height = WINDOW_HEIGHT;
    }
    else {
        gw.width = GetMonitorWidth(display);
        gw.height = GetMonitorHeight(display);
    }
    SetWindowSize(gw.width, gw.height);
    ToggleFullscreen();
}

GameWindow getWindow() {
    return gw;
}

void freeWindow() {
    CloseWindow();
}
