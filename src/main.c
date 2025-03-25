#include <stdlib.h>
#include <time.h>

#include "raylib.h"
#ifdef PLATFORM_WEB
#include <emscripten/emscripten.h>
#endif // PLATFORM_WEB

#include "window.h"
#include "game.h"
#include "sounds.h"
#include "world.h"
#include "player.h"
#include "inventory.h"
#include "textures.h"
#include "screen.h"
#include "title.h"

void loop() {
    if (IsKeyPressed(KEY_ENTER) && (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT)))
        toggleFullscreen();
    switch(getScreen()) {
    case TITLE:
        titleLoop();
        break;
    case GAMEPLAY:
        gameLoop();
        break;
    }
}

int main(void) {
    srand(time(NULL));

    initWindow();
    initTextures();

    initSounds();
    playSound(S_BACKGROUND1);

    initWorld();
    initPlayer();
    initInventory();
    initTitle();

#ifdef PLATFORM_WEB
    emscripten_set_main_loop(loop, 0, 1);
    if (WindowShouldClose()) {
        emscripten_cancel_main_loop();
        CloseWindow();
    }
#else
    while (!WindowShouldClose()) {
        loop();
    }
#endif // PLATFORM_WEB

    freeWindow();
    freePlayer();
    freeWorld();
    freeInventory();
    freeTextures();
    freeTitle();
    freeSounds();

    return 0;
}
