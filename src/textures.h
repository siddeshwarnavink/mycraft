/*
 * Manage dynamically rendered textures efficiently.
 */
#ifndef TEXTURES_H
#define TEXTURES_H

#include <stdint.h>

#include "raylib.h"

#include "blocks.h"
#include "state.h"

#define MAX_TEXTURES 20

enum TextureType {
    T_INVALID = -1,
    T_HUD_BLOCK,
    T_HAND_BLOCK,
    T_HAND
};

typedef union {
    enum BlockType block;
} TextureSubtype;

typedef struct {
    enum TextureType type;
    TextureSubtype subtype;
    RenderTexture2D renderTexture;
} TextureEntry;

typedef struct {
    TextureEntry entries[MAX_TEXTURES];
    uint8_t size;
} Textures;

/*
 * Initialize the textures hashmap.
 */
void textures_init();

/*
 * Get render texture.
 */
RenderTexture2D textures_get(enum TextureType type, TextureSubtype subtype);

/*
 * Cleanup all textures.
 */
void textures_clean();

#endif // !TEXTURES_H
