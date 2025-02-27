#include "raylib.h"
#include "rlgl.h"

#include "textures.h"

/*
 * Use the sprite texture to draw a 3D cube.
 */
static void _render_block(float size, Vector2 textureCoord, Vector3 position) {
	const float x = position.x;
	const float y = position.y;
	const float z = position.z;

	const float width = size;
	const float height = size;
	const float length = size;

	const float texWidth = (float)getSprite().width;
	const float texHeight = (float)getSprite().height;

	Rectangle source = { textureCoord.x, textureCoord.y, 32, 32 };

	const Color color = WHITE;

	rlSetTexture(getSprite().id);

	rlBegin(RL_QUADS);
	rlColor4ub(color.r, color.g, color.b, color.a);

	// Front face
	rlNormal3f(0.0f, 0.0f, 1.0f);
	rlTexCoord2f(source.x/texWidth, (source.y + source.height)/texHeight);
	rlVertex3f(x - width/2, y - height/2, z + length/2);
	rlTexCoord2f((source.x + source.width)/texWidth, (source.y + source.height)/texHeight);
	rlVertex3f(x + width/2, y - height/2, z + length/2);
	rlTexCoord2f((source.x + source.width)/texWidth, source.y/texHeight);
	rlVertex3f(x + width/2, y + height/2, z + length/2);
	rlTexCoord2f(source.x/texWidth, source.y/texHeight);
	rlVertex3f(x - width/2, y + height/2, z + length/2);

	// Back face
	rlNormal3f(0.0f, 0.0f, - 1.0f);
	rlTexCoord2f((source.x + source.width)/texWidth, (source.y + source.height)/texHeight);
	rlVertex3f(x - width/2, y - height/2, z - length/2);
	rlTexCoord2f((source.x + source.width)/texWidth, source.y/texHeight);
	rlVertex3f(x - width/2, y + height/2, z - length/2);
	rlTexCoord2f(source.x/texWidth, source.y/texHeight);
	rlVertex3f(x + width/2, y + height/2, z - length/2);
	rlTexCoord2f(source.x/texWidth, (source.y + source.height)/texHeight);
	rlVertex3f(x + width/2, y - height/2, z - length/2);

	// Top face
	rlNormal3f(0.0f, 1.0f, 0.0f);
	rlTexCoord2f(source.x/texWidth, source.y/texHeight);
	rlVertex3f(x - width/2, y + height/2, z - length/2);
	rlTexCoord2f(source.x/texWidth, (source.y + source.height)/texHeight);
	rlVertex3f(x - width/2, y + height/2, z + length/2);
	rlTexCoord2f((source.x + source.width)/texWidth, (source.y + source.height)/texHeight);
	rlVertex3f(x + width/2, y + height/2, z + length/2);
	rlTexCoord2f((source.x + source.width)/texWidth, source.y/texHeight);
	rlVertex3f(x + width/2, y + height/2, z - length/2);

	// Bottom face
	rlNormal3f(0.0f, - 1.0f, 0.0f);
	rlTexCoord2f((source.x + source.width)/texWidth, source.y/texHeight);
	rlVertex3f(x - width/2, y - height/2, z - length/2);
	rlTexCoord2f(source.x/texWidth, source.y/texHeight);
	rlVertex3f(x + width/2, y - height/2, z - length/2);
	rlTexCoord2f(source.x/texWidth, (source.y + source.height)/texHeight);
	rlVertex3f(x + width/2, y - height/2, z + length/2);
	rlTexCoord2f((source.x + source.width)/texWidth, (source.y + source.height)/texHeight);
	rlVertex3f(x - width/2, y - height/2, z + length/2);

	// Right face
	rlNormal3f(1.0f, 0.0f, 0.0f);
	rlTexCoord2f((source.x + source.width)/texWidth, (source.y + source.height)/texHeight);
	rlVertex3f(x + width/2, y - height/2, z - length/2);
	rlTexCoord2f((source.x + source.width)/texWidth, source.y/texHeight);
	rlVertex3f(x + width/2, y + height/2, z - length/2);
	rlTexCoord2f(source.x/texWidth, source.y/texHeight);
	rlVertex3f(x + width/2, y + height/2, z + length/2);
	rlTexCoord2f(source.x/texWidth, (source.y + source.height)/texHeight);
	rlVertex3f(x + width/2, y - height/2, z + length/2);

	// Left face
	rlNormal3f( - 1.0f, 0.0f, 0.0f);
	rlTexCoord2f(source.x/texWidth, (source.y + source.height)/texHeight);
	rlVertex3f(x - width/2, y - height/2, z - length/2);
	rlTexCoord2f((source.x + source.width)/texWidth, (source.y + source.height)/texHeight);
	rlVertex3f(x - width/2, y - height/2, z + length/2);
	rlTexCoord2f((source.x + source.width)/texWidth, source.y/texHeight);
	rlVertex3f(x - width/2, y + height/2, z + length/2);
	rlTexCoord2f(source.x/texWidth, source.y/texHeight);
	rlVertex3f(x - width/2, y + height/2, z - length/2);

	rlEnd();

	rlSetTexture(0);
}

/*
 * Get the position of texture in the sprite.
 */
Vector2 _sprite_pos(enum BlockType block) {
    switch (block) {
        case B_BEDROCK:
            return (Vector2){ 96, 0 };
        case B_STONE:
            return (Vector2){ 64, 0 };
        case B_DIRT:
            return (Vector2){ 32, 0 };
        case B_GRASS:
            return (Vector2){ 0, 0 };
        default:
            return (Vector2){ 0, 0 };
    }
}

void drawBlock(enum BlockType block, Vector3 position) {
    _render_block(1.0f, _sprite_pos(block), position);
}

void drawHudBlock(enum BlockType block, Vector3 position) {
    _render_block(45.0f, _sprite_pos(block), position);
}

void drawHandBlock(enum BlockType block, Vector3 position) {
    _render_block(50.0f, _sprite_pos(block), position);
}
