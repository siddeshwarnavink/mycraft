#include "inventory.h"

void addItem(Inventory *inv, enum BlockType itm) {
    if(inv->size >= INVENTORY_SIZE) return;

    // add to existing slot
    for(int i = 0; i < inv->size; ++i) {
        if (inv->items[i] == itm) {
            if (inv->qty[i] < 64) {
                ++inv->qty[i];
                return;
            }
        }
    }

    // create new slot
    inv->items[inv->size] = itm;
    inv->qty[inv->size] = 1;
    ++inv->size;
}

void removeOneItem(Inventory *inv, enum BlockType itm) {
    for (int i = 0; i < inv->size; ++i) {
        if (inv->items[i] == itm) {
            if (inv->qty[i] > 0) {
                --inv->qty[i];
                if (inv->qty[i] == 0) {
                    // shift remaining items to the left
                    for (int j = i; j < inv->size - 1; ++j) {
                        inv->items[j] = inv->items[j + 1];
                        inv->qty[j] = inv->qty[j + 1];
                    }
                    --inv->size;
                }
                return;
            }
        }
    }
}
