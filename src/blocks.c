#include "raylib.h"
#include "rlgl.h"

#include "state.h"

static void _draw_block(Vector2 textureCoord, Vector3 position) {
    const float x = position.x;
    const float y = position.y;
    const float z = position.z;

    const float width = 1.0f;
    const float height = 1.0f;
    const float length = 1.0f;

    const float texWidth = (float)texture.width;
    const float texHeight = (float)texture.height;

    Rectangle source = { textureCoord.x, textureCoord.y, 32, 32 };

    const Color color = WHITE;

    rlSetTexture(texture.id);

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

void drawGrass(Vector3 position) {
	_draw_block((Vector2){ 0, 0 }, position);
}
