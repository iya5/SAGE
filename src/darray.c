/* SAGE: Sage Ain't A Game Engine. An OpenGL 3D Renderer.

This file is part of Sage

Sage is free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.

Sage is distributed in the hope that it will be useful, but WITHOUT ANY 
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with 
Sage; see the file LICENSE. If not, see <https://www.gnu.org/licenses/>.    */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "logger.h"
#include "assert.h"
#include "darray.h"

darray *darray_alloc(size_t n_size, size_t initial_size)
{
    darray *arr = malloc(sizeof(darray));
    if (arr == NULL) goto err;
    arr->len = 0;
    arr->capacity = initial_size;
    arr->item_size = n_size;
    arr->items = calloc(initial_size, n_size);
    if (arr->items == NULL) goto err;

    return arr;

err:
    if (arr) free(arr);
    SFATAL("Failed to alloc memory for a darray");
    return NULL;
}

size_t darray_push(darray *arr, void *item)
{
    if (arr->len == arr->capacity) {
        arr->capacity = (arr->capacity > 0)
            ? arr->capacity * DARRAY_RESIZE_FACTOR
            : 1;
        void *items= realloc(arr->items, arr->capacity * arr->item_size);
        if (items == NULL) SFATAL("Failed to realloc darray"); /* handle behaviour */
        arr->items = items;
    }
    size_t index = arr->len;
    size_t offset = index * arr->item_size;
    memcpy((uint8_t  *) arr->items + offset, item, arr->item_size);
    arr->len++;

    return index;
}

void *darray_pop(darray *arr)
{
    SASSERT(arr->len > 0);
    void *item = darray_at(arr, arr->len - 1);
    arr->len--;
    return item;
}


void *darray_pop_at(darray *arr,  size_t index)
{
    SASSERT(index >= 0 && index <= arr->capacity);
    SASSERT(arr->len > 0);

    void *item = darray_at(arr, index);

    if (arr->len == 1) {
        arr->len--;
        return item;
    }

    /* copy array from the right of popped and shift down by size of item */
    size_t offset_diff = arr->len - index - 1;
    void *items_at_index = (uint8_t *) arr->items + index * arr->item_size;
    void *items_to_the_right = (uint8_t *) arr->items + (index + 1) * arr->item_size;
    memcpy(items_at_index, items_to_the_right, offset_diff);
    arr->len--;

    return item;
}

void *darray_at(darray *arr, size_t index)
{
    SASSERT(index >= 0 && index < arr->len);
    SASSERT(arr->len > 0);

    size_t offset = index * arr->item_size;
    void *item = (uint8_t *) arr->items + offset;
    return item;
}

void darray_free(darray *arr)
{
    free(arr->items);
    arr->items = NULL;
    free(arr);
}
