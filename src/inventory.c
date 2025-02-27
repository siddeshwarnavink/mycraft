#include <stdio.h>

#include "inventory.h"
#include "window.h"
#include "textures.h"

static Inventory inv;

void renderInventory() {
    const float x = getWindow().width/2 - 170;
    const float y = getWindow().height - 50;
    const float size = 40;

    TextureSubtype subtype;
    RenderTexture2D target;

    for (int i = 0; i < INVENTORY_SIZE; ++i) {
        if(getInv().selected == i)
            DrawRectangle(x+ i * (size+ 5), y, size, size, LIGHTGRAY);
        else
            DrawRectangle(x+ i * (size+ 5), y, size, size, GRAY);
        DrawRectangleLines(x + i * (size + 5), y, size, size, BLACK);
        if (i < getInv().size) {
            char countText[4];
            snprintf(countText, sizeof(countText), "%d", getInv().qty[i]);
            DrawText(countText, x + i * (size+ 5) + size - 12, y, 12, BLACK);

            subtype.block = getInv().items[i];
            target = getTexture(T_HUD_BLOCK, subtype);
            DrawTexture(target.texture, x + i * (size+ 5), y, WHITE);
        }
    }
}

void invAddItem(enum BlockType itm) {
    if(inv.size >= INVENTORY_SIZE) return;

    // add to existing slot
    for(int i = 0; i < inv.size; ++i) {
        if (inv.items[i] == itm) {
            if (inv.qty[i] < 64) {
                ++inv.qty[i];
                return;
            }
        }
    }

    // create new slot
    inv.items[inv.size] = itm;
    inv.qty[inv.size] = 1;
    ++inv.size;
}

void invRemoveOneItem(enum BlockType itm) {
    for (int i = 0; i < inv.size; ++i) {
        if (inv.items[i] == itm) {
            if (inv.qty[i] > 0) {
                --inv.qty[i];
                if (inv.qty[i] == 0) {
                    // shift remaining items to the left
                    for (int j = i; j < inv.size - 1; ++j) {
                        inv.items[j] = inv.items[j + 1];
                        inv.qty[j] = inv.qty[j + 1];
                    }
                    --inv.size;
                }
                return;
            }
        }
    }
}


Inventory getInv() {
    return inv;
}

void initInventory() {
    inv.selected = 0;
}

void setInvSelected(int p) {
    inv.selected = p;
}

void freeInventory() {
    // :)
}
