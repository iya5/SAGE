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

#ifndef SAGE_DARRAY_H
#define SAGE_DARRAY_H

#include <stddef.h>

#define DARRAY_RESIZE_FACTOR 2

typedef struct darray {
    size_t len;
    size_t capacity;
    size_t item_size;
    void *items;
} darray;

darray *darray_alloc(size_t n_size, size_t initial_size);
size_t darray_push(darray *arr, void *item);
void *darray_pop_at(darray *arr,  size_t index);
void *darray_pop(darray *arr);
void *darray_at(darray *arr, size_t index);
void darray_free(darray *arr);

#endif /* SAGE_DARRAY_H */
