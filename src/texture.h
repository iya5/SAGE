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

enum texture_type {
    /*
     * Clearing some confusion and a personal note, a diffuse map or texture map
     * is really just the actual image attached to a mesh, and it's called
     * diffuse because the texture is used directly for the diffuse shading
     * model in the fragment shader. I was always wondering what a "normal"
     * texture is called---"normal" relating to just a plain texture attached to
     * a mesh, but it's really just called a diffuse map.
     *
     * Calling it diffuse makes it sound like some special material property but
     * it's just the base texture and is what gives it actual visual "form"
     * because it reacts to light!
     */
    TEXTURE_DIFFUSE,
    TEXTURE_SPECULAR,
    TEXTURE_CUBEMAP
};

struct texture {
    uint32_t id;
    enum texture_type type;
    int32_t width;
    int32_t height;
};

struct texture texture_create_default(void);
struct texture texture_create(const char *path, enum texture_type type);

/* Wrapper around glBindTexture() */
void texture_bind(struct texture t, size_t texture_unit);
void texture_destroy(struct texture *t);

struct texture cubemap_texture_create(char *cubemap_faces[6]);
void cubemap_texture_bind(struct texture t);

#endif /* SAGE_TEXTURE_H */
