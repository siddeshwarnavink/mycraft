/*
 * Inventory management logic
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
} Inventory;


/*
 * Add item to inventory.
 */
void addItem(Inventory *inv, enum BlockType itm);


/*
 * Remove one item from inventory.
 */
void removeOneItem(Inventory *inv, enum BlockType itm);

#endif // !INVENTORY_H
