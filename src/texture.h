/* Texture header file for Sage

This file is part of Sage

Sage is free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.

Sage is distributed in the hope that it will be useful, but WITHOUT ANY 
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with 
Sage; see the file LICENSE. If not, see <https://www.gnu.org/licenses/>.    */

#ifndef __SAGE_TEXTURE_H__
#define __SAGE_TEXTURE_H__

#include <stdint.h>
#include "mnf/mnf_types.h"

struct texture {
    uint32_t id;
    ivec2 size;
};

struct texture texture_create_default(void);
struct texture texture_create(const char *path);
/* Wrapper around glBindTexture() */
void texture_bind(struct texture t);
void texture_destroy(struct texture *t);

struct texture cubemap_texture_create(char *cubemap_faces[6]);
void cubemap_texture_bind(struct texture t);

#endif /* __SAGE_TEXTURE_H__ */
