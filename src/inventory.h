/*
 * Player HUD inventory.
 */
#ifndef INVENTORY_H
#define INVENTORY_H

#define INVENTORY_SIZE 8

#include <stdint.h>

#include "raylib.h"

#include "blocks.h"

typedef struct {
    uint8_t size;
    enum BlockType items[INVENTORY_SIZE];
    uint8_t qty[INVENTORY_SIZE];
    int selected;
} Inventory;

Inventory getInv();

void initInventory();

void freeInventory();

/*
 * Set selected item in inventory.
 */
void setInvSelected(int p);

/*
 * Render inventory HUD
 */
void renderInventory();

/*
 * Add item to inventory.
 */
void invAddItem(enum BlockType itm);

/*
 * Remove one item from inventory.
 */
void invRemoveOneItem(enum BlockType itm);

#endif // !INVENTORY_H
