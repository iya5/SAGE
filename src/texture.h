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

#ifndef SAGE_TEXTURE_H
#define SAGE_TEXTURE_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

struct texture {
    uint32_t id;
    int32_t width;
    int32_t height;
};

struct texture texture_create_default(void);
struct texture texture_create(const char *path);
/* Same function as texture_create() except it returns the ID instead of a
   texture struct */
uint32_t texture_create_id(const char *path, bool flip);
/* Wrapper around glBindTexture() */
void texture_bind(struct texture t, size_t texture_unit);
/* Wrapper around texture_destroy_id */
void texture_destroy(struct texture *t);
void texture_destroy_id(uint32_t *id);

struct texture cubemap_texture_create(const char *cubemap_faces[6]);
void cubemap_texture_bind(struct texture t);

#endif /* SAGE_TEXTURE_H */
