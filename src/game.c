#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"

#include "window.h"
#include "world.h"
#include "player.h"
#include "game.h"
#include "blocks.h"
#include "sounds.h"
#include "inventory.h"
#include "textures.h"

static uint8_t debugMode   = 0;
static uint8_t isHolding   = 0;
static float holdTime      = 0.0f;
static float mouseThrottle = 0.0f;

/*
 * Block selection logic.
 */
static void _block_selection() {
	Ray crosshairRay              = GetScreenToWorldRay((Vector2){ getWindow().width/2, getWindow().height/2 }, getPlayer().camera);
	RayCollision closestCollision = { 0 };
	closestCollision.distance     = INFINITY;
	Vector3Int newSelected        = { -1, -1, -1 };
	const float maxRange          = 5.0f;

	for (int depth = 0; depth < WORLD_HEIGHT; ++depth) {
		for (int length = 0; length < WORLD_LENGTH; ++length) {
			for (int breath = 0; breath < WORLD_BREADTH; ++breath) {
				if (getWorld().blocks[depth][length][breath] == B_VOID) continue;

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
        setBlockSelected(newSelected);
	} else {
		newSelected.x = -1;
        setBlockSelected(newSelected);
	}
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

                invAddItem(getWorld().blocks[getPlayer().blockSelected.y][getPlayer().blockSelected.x][getPlayer().blockSelected.z]);
                updateWorldBlock(getPlayer().blockSelected.y, getPlayer().blockSelected.x, getPlayer().blockSelected.z, B_VOID);
                isHolding = 0;
            }
        }
    } else isHolding = 0;

    // Right click (place block)
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && mouseThrottle == 0.0f) {
        mouseThrottle += GetFrameTime();
        if(getInv().selected < getInv().size && getPlayer().blockSelected.y + 1 <= WORLD_HEIGHT) {
            updateWorldBlock(getPlayer().blockSelected.y +1, getPlayer().blockSelected.x, getPlayer().blockSelected.z, getInv().items[getInv().selected]);
            invRemoveOneItem(getInv().items[getInv().selected]);
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
        setInvSelected(getInv().selected - 1);
        if(getInv().selected < 0)
            setInvSelected(INVENTORY_SIZE - 1);
    }
    else if (scroll < 0.0f) {
        setInvSelected(getInv().selected + 1);
        if(getInv().selected > INVENTORY_SIZE - 1)
            setInvSelected(0);
    }

    handleGravity();
    handleCollision();

    BeginDrawing();

    ClearBackground(SKYBLUE);

    BeginMode3D(getPlayer().camera);

    // Player hitbox
    if(debugMode)
        DrawCubeWires(getPlayer().camera.position, PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_WIDTH, DARKPURPLE);

    _block_selection();

    renderWorld();

    EndMode3D();

    renderPlayer();
    renderInventory();

    // Crosshair
    DrawLine(getWindow().width / 2 - 10, getWindow().height / 2, getWindow().width / 2 + 10, getWindow().height / 2, RED);
    DrawLine(getWindow().width / 2, getWindow().height / 2 - 10, getWindow().width / 2, getWindow().height / 2 + 10, RED);

    // Debug
    if(debugMode) {
        DrawText(TextFormat("Selected: %d, %d, %d", getPlayer().blockSelected.x, getPlayer().blockSelected.y, getPlayer().blockSelected.z), 20, 10, 10, BLACK);
        DrawText(TextFormat("Camera position: %.2f, %.2f, %.2f", getPlayer().camera.position.x, getPlayer().camera.position.y, getPlayer().camera.position.z), 20, 20, 10, BLACK);
        DrawText(TextFormat("Camera target: %.2f, %.2f, %.2f", getPlayer().camera.target.x, getPlayer().camera.target.y, getPlayer().camera.target.z), 20, 30, 10, BLACK);
        DrawText(TextFormat("Mouse holding: %d", isHolding), 20, 40, 10, BLACK);
        DrawText(TextFormat("Mouse holding duration: %.2f", holdTime), 20, 50, 10, BLACK);
        DrawText(TextFormat("Block under: %d", blockUnderPlayer()), 20, 60, 10, BLACK);
        DrawText(TextFormat("Player colliding: %d", playerColliding()), 20, 70, 10, BLACK);
        DrawText(TextFormat("Mouse scroll: %f", scroll), 20, 80, 10, BLACK);
        DrawText(TextFormat("HUD cursor position: %d", getInv().selected), 20, 90, 10, BLACK);
    }

    EndDrawing();
}
