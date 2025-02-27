#include <stdlib.h>
#include <time.h>

#include "raylib.h"

#include "window.h"
#include "game.h"
#include "sounds.h"
#include "world.h"
#include "player.h"
#include "inventory.h"
#include "textures.h"

int main(void) {
    srand(time(NULL));

    initWindow();
    initTextures();

    initSounds();
    playSound(S_BACKGROUND1);

    initWorld();
    initPlayer();
    initInventory();

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_ENTER) && (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT)))
            toggleFullscreen();
        gameLoop();
    }

    freeWindow();
    freePlayer();
    freeWorld();
    freeInventory();
    freeTextures();
    freeSounds();

    return 0;
}
