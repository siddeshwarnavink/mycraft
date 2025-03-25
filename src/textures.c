#include <stdio.h>

#include "window.h"
#include "textures.h"
#include "inventory.h"

static Textures textures;
static Texture2D sprite;

/*
 * Find index given type and subtype.
 */
static int _find_texture(enum TextureType type, TextureSubtype subtype) {
    for (int i = 0; i < textures.size; i++) {
        if (textures.entries[i].type == type && textures.entries[i].subtype.block == subtype.block)
            return i;
    }
    return -1;
}

/*
 * Handle rendering right hand.
 */
static RenderTexture2D _render_hand() {
    RenderTexture2D target = LoadRenderTexture(getWindow().width/2, getWindow().width/2);

    BeginTextureMode(target);

    ClearBackground((Color){ 0, 0, 0, 0 });

    BeginMode3D((Camera) {
            .position = { 20.0f, 10.0f, 10.0f },
            .target = { 0.0f, 0.0f, 0.0f },
            .up = { 0.0f, 1.0f, 10.0f },
            .fovy = 45.0f,
            .projection = CAMERA_PERSPECTIVE
            });

    const float hand_width  = 10.0f;
    const float hand_length = 3.5f;

    DrawCube((Vector3){0, 0, 0}, hand_length, hand_length, hand_width, BROWN);
    DrawCubeWires((Vector3){0, 0, 0}, hand_length, hand_length, hand_width, DARKGRAY);

    EndMode3D();

    EndTextureMode();
    return target;
}

/*
 * Handle rendering HUD block.
 */
static RenderTexture2D _render_hud_block(enum BlockType block) {
    RenderTexture2D target = LoadRenderTexture(40, 40);

    BeginTextureMode(target);

    ClearBackground((Color){ 0, 0, 0, 0 });

    BeginMode3D((Camera) {
            .position = { 100.0f, -50.0f, 100.0f },
            .target = { 0.0f, 2.0f, 0.0f },
            .up = { 0.0f, 1.0f, 0.0f },
            .fovy = 45.0f,
            .projection = CAMERA_PERSPECTIVE
            });

    drawHudBlock(block, (Vector3){0, 0, 0});

    EndMode3D();

    EndTextureMode();

    return target;
}

/*
 * Handle rendering block at player's hand.
 */
static RenderTexture2D _render_hand_block(enum BlockType block) {
    RenderTexture2D target = LoadRenderTexture(getWindow().width/2, getWindow().width/2);

    BeginTextureMode(target);

    ClearBackground((Color){ 0, 0, 0, 0 });

    BeginMode3D((Camera) {
            .position = { 100.0f, -100.0f, 100.0f },
            .target = { 0.0f, 2.0f, 0.0f },
            .up = { 0.0f, 1.0f, 0.0f },
            .fovy = 45.0f,
            .projection = CAMERA_PERSPECTIVE
            });

    drawHandBlock(getInv().items[getInv().selected], (Vector3){0, 0, 0});

    const float size = 50.0f;
    DrawCubeWires((Vector3){0, 0, 0}, size, size, size, BLACK);

    EndMode3D();

    EndTextureMode();

    return target;
}

/*
 * Render appropriate texture given type and subtype.
 */
static RenderTexture2D _render_texture(enum TextureType type, TextureSubtype subtype) {
    rlPushMatrix();
    switch(type) {
        case T_HUD_BLOCK:
            printf("[info] Rendering T_HUD_BLOCK %d\n", subtype.block);
            return _render_hud_block(subtype.block);
            break;
        case T_HAND_BLOCK:
            printf("[info] Rendering T_HAND_BLOCK %d\n", subtype.block);
            return _render_hand_block(subtype.block);
            break;
        case T_HAND:
            printf("[info] Rendering T_HAND\n");
            return _render_hand();
            break;
        default:
            printf("[error] Invalid texture type\n");
            return (RenderTexture2D){0};
    }
    rlPopMatrix();
}

Texture2D getSprite() {
    return sprite;
}

void initTextures() {
    sprite = LoadTexture("resources/texture.png");
    textures.size = 0;
    for (int i = 0; i < MAX_TEXTURES; i++)
        textures.entries[i].type = -1;
}

RenderTexture2D getTexture(enum TextureType type, TextureSubtype subtype) {
    int index = _find_texture(type, subtype);
    if (index != -1) {
        return textures.entries[index].renderTexture;
    }
    if (textures.size >= MAX_TEXTURES) {
        printf("[error] Texture limit reached\n");
        return (RenderTexture2D){0};
    }

    TextureEntry newEntry;
    newEntry.type = type;
    newEntry.subtype = subtype;
    newEntry.renderTexture = _render_texture(type, subtype);
    textures.entries[textures.size] = newEntry;
    textures.size++;
    return newEntry.renderTexture;
}

void freeTextures() {
    UnloadTexture(sprite);
    for (int i = 0; i < textures.size; i++) {
        UnloadRenderTexture(textures.entries[i].renderTexture);
    }
    textures.size = 0;
}
