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
#include "darray.h"

struct vertex {
    vec3 pos;
    vec3 normal;
    vec2 uv;
};

struct mesh_gpu {
    uint32_t vao;
    uint32_t vbo;
    uint32_t ibo;
    uint32_t vertex_count;
    uint32_t index_count;
};

struct transform {
    vec3 rotation;
    vec3 scale;
    vec3 position;
};

struct mesh {
    struct mesh_gpu buffer;
    darray *vertices;
    darray *indices;
    darray *textures;
    mat4 model;
};

struct mesh mesh_geometry_create_2d_triangle(void);
struct mesh mesh_geometry_create_quad(void);
struct mesh mesh_geometry_create_cube(void);
struct mesh mesh_create_from_vertices(darray *vertices);
void mesh_add_texture(struct mesh *mesh, struct texture texture);
void mesh_destroy(struct mesh *mesh);
void mesh_bind(struct mesh mesh);
void mesh_draw(struct mesh mesh);

void transform_reset(struct transform *transform);
void transform_scale(struct transform *transform, vec3 scalars);
void transform_rotation(struct transform *transform, vec3 euler_angles);
void transform_position(struct transform *transform, vec3 position);
void transform_model(struct transform transform, mat4 out);

#endif /* SAGE_MESH_H */
