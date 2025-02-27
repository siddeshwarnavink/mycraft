/*
 * Manage the GUI game window.
 */
#ifndef WINDOW_H
#define WINDOW_H

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 450

typedef struct {
    int width;
    int height;
} GameWindow;

GameWindow getWindow();

/*
 * Unnecessary abstraction of raylib window.
 */
void initWindow();

/*
 * Another unnecessary abstraction of raylib window.
 */
void freeWindow();

/*
 * Toggle between fullscreen and window mode.
 */
void toggleFullscreen();

#endif // !WINDOW_H
