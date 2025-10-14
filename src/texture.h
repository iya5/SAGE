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
#include <cglm/cglm.h>

struct texture {
    uint32_t id;
    ivec2 size;
};

void texture_bind(struct texture t);
struct texture texture_create(const char *path);
void texture_destroy(struct texture *t);

#endif /* __SAGE_TEXTURE_H__ */
