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

#ifndef SAGE_MESH_H
#define SAGE_MESH_H

#include <stdint.h>
#include <stddef.h>

#include "mnf/mnf_types.h"
#include "texture.h"

struct vertex {
    vec3 position;
    vec3 normal;
    vec3 uv;
};

struct vertex_array {
    uint32_t vao;
    uint32_t vbo;
    uint32_t vertex_count;
};

struct transform {
    vec3 rotation;
    vec3 scale;
    vec3 position;
};

struct mesh {
    struct vertex_array vertex_array;
    mat4 model;
};

struct mesh mesh_create(const float *vertices, size_t n_bytes);
void mesh_destroy(struct mesh *mesh);
void mesh_bind(struct mesh mesh);
void mesh_draw(struct mesh mesh);

void transform_reset(struct transform *transform);
void transform_scale(struct transform *transform, vec3 scalars);
void transform_rotation(struct transform *transform, vec3 euler_angles);
void transform_position(struct transform *transform, vec3 position);
void transform_model(struct transform transform, mat4 out);

#endif /* SAGE_MESH_H */
