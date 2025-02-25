#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"

#include "game.h"
#include "core.h"
#include "state.h"
#include "blocks.h"
#include "sounds.h"
#include "inventory.h"
#include "textures.h"

/*
 * Block selection logic.
 */
static void _block_selection() {
	Ray crosshairRay              = GetScreenToWorldRay((Vector2){ width/2, height/2 }, camera);
	RayCollision closestCollision = { 0 };
	closestCollision.distance     = INFINITY;
	Vector3Int newSelected        = { -1, -1, -1 };
	const float maxRange          = 5.0f;

	for (int depth = 0; depth < WORLD_HEIGHT; ++depth) {
		for (int length = 0; length < WORLD_LENGTH; ++length) {
			for (int breath = 0; breath < WORLD_BREADTH; ++breath) {
				if (world[depth][length][breath] == B_VOID) continue;

				Vector3 pos = (Vector3){ (float)length, (float)depth, (float)breath };
				BoundingBox box = {
					.min = { pos.x - 0.5f, pos.y - 0.5f, pos.z - 0.5f },
					.max = { pos.x + 0.5f, pos.y + 0.5f, pos.z + 0.5f }
				};

				RayCollision col = GetRayCollisionBox(crosshairRay, box);

				if (col.hit && col.distance <= maxRange && col.distance < closestCollision.distance) {
					closestCollision = col;
					newSelected.x = length;
					newSelected.y = depth;
					newSelected.z = breath;
				}
			}
		}
	}

	if (closestCollision.hit) {
		selected = newSelected;
	} else {
		selected.x = -1;
	}
}

/*
 * Render the world as we see.
 */
static void _render_world() {
    for (int depth = 0; depth < WORLD_HEIGHT; ++depth) {
        for (int length = 0; length < WORLD_LENGTH; ++length) {
            for (int breath = 0; breath < WORLD_BREADTH; ++breath) {
                if(world[depth][length][breath] == B_VOID) continue;

                Vector3 pos = (Vector3){ (float)length, (float)depth, (float)breath };

                if (length == selected.x && depth == selected.y && breath == selected.z) {
                    DrawCubeWires(pos, 1.0f, 1.0f, 1.0f, WHITE);
                } else {
                    DrawCubeWires(pos, 1.0f, 1.0f, 1.0f, BLACK);
                }

                drawBlock(world[depth][length][breath], pos);
            }
        }
    }
}

/*
 * Draw the inventory HUD.
 */
static void _render_hud() {
    const float x = width/2 - 170;
    const float y = height - 50;
    const float size = 40;

    TextureSubtype subtype;
    RenderTexture2D target;

    for (int i = 0; i < INVENTORY_SIZE; ++i) {
        if(hudPos == i)
            DrawRectangle(x+ i * (size+ 5), y, size, size, LIGHTGRAY);
        else
            DrawRectangle(x+ i * (size+ 5), y, size, size, GRAY);
        DrawRectangleLines(x + i * (size + 5), y, size, size, BLACK);
        if (i < inv.size) {
            char countText[4];
            snprintf(countText, sizeof(countText), "%d", inv.qty[i]);
            DrawText(countText, x + i * (size+ 5) + size - 12, y, 12, BLACK);

            subtype.block = inv.items[i];
            target = textures_get(T_HUD_BLOCK, subtype);
            DrawTexture(target.texture, x + i * (size+ 5), y, WHITE);
        }
    }
}


/*
 * Draw right hand if a block is selected in HUD
 * or draw the block in hand.
 */
static void _render_hand() {
	RenderTexture2D target;
	TextureSubtype subtype;
	if(hudPos < inv.size) {
		subtype.block = inv.items[hudPos];
		target = textures_get(T_HAND_BLOCK, subtype);
	}
	else
		target = textures_get(T_HAND, subtype);
	DrawTexture(target.texture, width - (width/2), height - (height/2), WHITE);
}

void gameLoop() {
    // Debug toggle
    if (IsKeyPressed(KEY_F3)) debugMode = !debugMode;

    // Left click (break block)
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        if (!isHolding) {
            isHolding = 1;
            holdTime = 0.0f;
            playSound(S_PUNCH);
        } else {
            holdTime += GetFrameTime();
            if (holdTime >= 1.0f) { // 1 second
                                    // Break the block
                int randomNumber = rand() % 2;
                if (randomNumber == 0)
                    playSound(S_BREAK1);
                else
                    playSound(S_BREAK2);

                addItem(&inv, world[selected.y][selected.x][selected.z]);
                world[selected.y][selected.x][selected.z] = B_VOID;
                isHolding = 0;
            }
        }
    } else isHolding = 0;

    // Right click (place block)
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && mouseThrottle == 0.0f) {
        mouseThrottle += GetFrameTime();
        if(hudPos < inv.size && selected.y + 1 <= WORLD_HEIGHT) {
            world[selected.y + 1][selected.x][selected.z] = inv.items[hudPos];
            removeOneItem(&inv, inv.items[hudPos]);
        }
    }

    // Mouse throttle
    if (mouseThrottle > 0.0f && mouseThrottle < 0.5f) {
        mouseThrottle += GetFrameTime();
    } else {
        mouseThrottle = 0.0f;
    }

    // Mouse scroll
    float scroll = GetMouseWheelMove();
    if (scroll > 0.0f) {
        if(--hudPos < 0)
            hudPos = 7;
    }
    else if (scroll < 0.0f) {
        if(++hudPos > 7)
            hudPos = 0;
    }

    handleGravity();
    handleCollision();

    BeginDrawing();

    ClearBackground(SKYBLUE);

    BeginMode3D(camera);

    // Player hitbox
    if(debugMode)
        DrawCubeWires(camera.position, PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_WIDTH, DARKPURPLE);

    _block_selection();

    _render_world();

    EndMode3D();

    _render_hand();

    _render_hud();

    // Crosshair
    DrawLine(width / 2 - 10, height / 2, width / 2 + 10, height / 2, RED);
    DrawLine(width / 2, height / 2 - 10, width / 2, height / 2 + 10, RED);

    // Debug
    if(debugMode) {
        DrawText(TextFormat("Selected: %d, %d, %d", selected.x, selected.y, selected.z), 20, 10, 10, BLACK);
        DrawText(TextFormat("Camera position: %.2f, %.2f, %.2f", camera.position.x, camera.position.y, camera.position.z), 20, 20, 10, BLACK);
        DrawText(TextFormat("Camera target: %.2f, %.2f, %.2f", camera.target.x, camera.target.y, camera.target.z), 20, 30, 10, BLACK);
        DrawText(TextFormat("Mouse holding: %d", isHolding), 20, 40, 10, BLACK);
        DrawText(TextFormat("Mouse holding duration: %.2f", holdTime), 20, 50, 10, BLACK);
        DrawText(TextFormat("Block under: %d", blockUnderPlayer()), 20, 60, 10, BLACK);
        DrawText(TextFormat("Player colliding: %d", playerColliding()), 20, 70, 10, BLACK);
        DrawText(TextFormat("Mouse scroll: %f", scroll), 20, 80, 10, BLACK);
        DrawText(TextFormat("HUD cursor position: %d", hudPos), 20, 90, 10, BLACK);
    }

    EndDrawing();
}
