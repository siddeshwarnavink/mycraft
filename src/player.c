#include <math.h>
#include <stdlib.h>

#include "player.h"
#include "world.h"
#include "textures.h"
#include "inventory.h"
#include "window.h"
#include "sounds.h"
#include "blocks.h"

static Player p;

void initPlayer() {
    Camera cam = {
        .position = { 4.0f, (6 + PLAYER_HEIGHT), 4.0f },
        .target = { 0.0f, 2.0f, 0.0f },
        .up = { 0.0f, 1.0f, 0.0f },
        .fovy = 60.0f,
        .projection = CAMERA_PERSPECTIVE
    };
    p.camera = cam;
    p.verticalVelocity = 0.0f;
}

void renderPlayer() {
    RenderTexture2D target;
    TextureSubtype subtype;
    if(getInv().selected < getInv().size) {
        subtype.block = getInv().items[getInv().selected];
        target = getTexture(T_HAND_BLOCK, subtype);
    }
    else
        target = getTexture(T_HAND, subtype);
    DrawTexture(target.texture, getWindow().width - (getWindow().width/2), getWindow().height - (getWindow().height/2), WHITE);
}

enum BlockType blockUnderPlayer() {
    const float offset = -0.50;
	float feetY = p.camera.position.y - PLAYER_HEIGHT;
	int x = (int)floor(p.camera.position.x - offset);
	int y = (int)floor(feetY);
	int z = (int)floor(p.camera.position.z - offset);

	if (x < 0 || x >= WORLD_LENGTH || z < 0 || z >= WORLD_BREADTH || y < 0 || y >= WORLD_HEIGHT)
		return B_VOID;

	return getWorld().blocks[y][x][z];
}

static BoundingBox _getBlockBoundingBox(Vector3 position) {
    return (BoundingBox){
        .min = { position.x - 0.5f, position.y - 0.5f, position.z - 0.5f },
        .max = { position.x + 0.5f, position.y + 0.5f, position.z + 0.5f }
    };
}

uint8_t playerColliding(Vector3 oldPos) {
    for (int depth = 0; depth < WORLD_HEIGHT; ++depth) {
        for (int length = 0; length < WORLD_LENGTH; ++length) {
            for (int breath = 0; breath < WORLD_BREADTH; ++breath) {
                if(getWorld().blocks[depth][length][breath] == B_VOID) continue;

                Vector3 block = (Vector3){ (float)length, (float)depth, (float)breath };
                Vector3 player = p.camera.position;

                BoundingBox blockBox = _getBlockBoundingBox(block);
                BoundingBox playerBox = (BoundingBox){
                    .min = { player.x - PLAYER_WIDTH / 2, player.y - PLAYER_HEIGHT, player.z - PLAYER_WIDTH / 2 },
                        .max = { player.x + PLAYER_WIDTH / 2, player.y, player.z + PLAYER_WIDTH / 2 }
                };

                if (CheckCollisionBoxes(blockBox, playerBox)) {
                    BoundingBox oldPlayerBox = (BoundingBox){
                        .min = { oldPos.x - PLAYER_WIDTH / 2, oldPos.y - PLAYER_HEIGHT, oldPos.z - PLAYER_WIDTH / 2 },
                        .max = { oldPos.x + PLAYER_WIDTH / 2, oldPos.y, oldPos.z + PLAYER_WIDTH / 2 }
                    };
                    return !CheckCollisionBoxes(oldPlayerBox, blockBox);
                }
            }
        }
    }
    return 0;
}

void handleCollisonAndGravity() {
    int onGround = (blockUnderPlayer() != 0);

    // Jumping
    if(IsKeyPressed(KEY_SPACE) && onGround) {
        int randomNumber = rand() % 2;
        if(randomNumber == 0)
            playSound(S_JUMP1);
        else
            playSound(S_JUMP2);

        p.verticalVelocity = JUMP_SPEED;
        onGround = 0;
    }

    // Gravity
    if(!onGround) {
        p.camera.position.y += p.verticalVelocity * GetFrameTime();
        p.verticalVelocity -= GRAVITY * GetFrameTime();
    } else
        p.verticalVelocity = 0.0f;

    // Collision
    Vector3 oldPos = p.camera.position;
    UpdateCamera(&p.camera, CAMERA_FIRST_PERSON);
    if(playerColliding(oldPos)) {
        if(!onGround)
            p.camera.position = oldPos;
        else if(p.verticalVelocity == 0.0f) {
            p.camera.position.x = oldPos.x;
            p.camera.position.z = oldPos.z;
        }
    }
}

void setBlockSelected(Vector3Int s) {
    p.blockSelected = s;
}

Player getPlayer() {
    return p;
}

void freePlayer() {
    // :)
}
